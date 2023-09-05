#include "EngineIncludes.h"

#include "VulkanInterface.h"
#include "../GoldDayEngine.h"

namespace gde {
	VulkanInterface::VulkanInterface(GoldDayEngine& _engine,Window& _window) : 
            engine(_engine), 
            window(_window),
            uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT),
            globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT) {
		engine.getDebugManager().getLogger().log(Logger::Verbose, "VulkanInterface Instantiated"); 

        recreateSwapChain();
		createCommandBuffers();

        createUbo();
        createDescriptorSets();
	};
	VulkanInterface::~VulkanInterface() {
        engine.getDebugManager().getLogger().log(Logger::Verbose, "VulkanInterface Freed");

        for (auto& data : pipelines) {
            vkDestroyPipelineLayout(device.device(), data.pipelineLayout, nullptr);
        }

        // pipelines need to be freed before VulkanInterface is destroyed
        pipelines.clear();

        freeCommandBuffers();
	}


    void VulkanInterface::recreateSwapChain() {
        auto extent = window.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = window.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(device.device());

        if (swapChain == nullptr) {
            swapChain = std::make_unique<SwapChain>(engine, device, extent);
        }
        else {
            std::shared_ptr<SwapChain> oldSwapChain = std::move(swapChain);
            swapChain = std::make_unique<SwapChain>(engine, device, extent, oldSwapChain);
            
            if (!oldSwapChain->compareSwapFormats(*swapChain.get())) {
                // replace w/ callback function to handle this edgecase later
                engine.getDebugManager().getLogger().log(Logger::Error, "Swap chain image(or depth) format has changed!");
            }
        }

        // fix this
    }

    void VulkanInterface::createCommandBuffers() {
        commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) !=
            VK_SUCCESS) {
            engine.getDebugManager().getLogger().log(Logger::Error, "failed to allocate command buffers!");
        }
    }

    void VulkanInterface::freeCommandBuffers() {
        vkFreeCommandBuffers(
            device.device(),
            device.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
    }

    void VulkanInterface::createUbo() {
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<Buffer>(
                device,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }
    }

    void VulkanInterface::createDescriptorSets() {
        globalPool =
            DescriptorPool::Builder(engine,device)
            .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();


        globalSetLayout =
            DescriptorSetLayout::Builder(engine,device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            DescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        globalSetLayout->getDescriptorSetLayout();
    }

    VkCommandBuffer VulkanInterface::beginFrame() {
        assert(!isFrameStarted && "Can't call beginFrame while already in progress");
        
        auto result = swapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            engine.getDebugManager().getLogger().log(Logger::Error, "failed to acquire swap chain image!");
        }

        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            engine.getDebugManager().getLogger().log(Logger::Error, "failed to begin recording command buffer!");
        }
        return commandBuffer;
    }
    void VulkanInterface::endFrame() {
        assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
        auto commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            engine.getDebugManager().getLogger().log(Logger::Error, "failed to record command buffer!");
        }

        auto result = swapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            window.wasWindowResized()) {
            window.resetWindowResizedFlag();
            recreateSwapChain();
            engine.getMOM().getSystem<system::CameraUpdate>()->setCameraAspectRatio();
        }
        else if (result != VK_SUCCESS) {
            engine.getDebugManager().getLogger().log(Logger::Error, "failed to present swap chain image!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
    }
    void VulkanInterface::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
        assert(
            commandBuffer == getCurrentCommandBuffer() &&
            "Can't begin render pass on command buffer from a different frame");
        
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain->getRenderPass();
        renderPassInfo.framebuffer = swapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{ {0, 0}, swapChain->getSwapChainExtent() };
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


    }
    void VulkanInterface::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
        assert(
            commandBuffer == getCurrentCommandBuffer() &&
            "Can't end render pass on command buffer from a different frame");
    
        vkCmdEndRenderPass(commandBuffer);

    }

    void VulkanInterface::setUboData(GlobalUbo& ubo) {
        uboBuffers[currentFrameIndex]->writeToBuffer(&ubo);
        uboBuffers[currentFrameIndex]->flush();
    }


    int VulkanInterface::createPipeline(PipelineType pipelineType) {

        PipelineData newData;

        VkPushConstantRange pushConstantRange{};
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout->getDescriptorSetLayout() };
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        PipelineConfigInfo pipelineConfig{};

        switch (pipelineType)
        {
        case gde::VulkanInterface::DEFAULT:
            
            pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            pushConstantRange.offset = 0;
            pushConstantRange.size = sizeof(DefaultPushConstantData);

            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
            pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
            pipelineLayoutInfo.pushConstantRangeCount = 1;
            pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
            if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &newData.pipelineLayout) !=
                VK_SUCCESS) {
                engine.getDebugManager().getLogger().log(Logger::Error, "failed to create pipeline layout!");
            }

            Pipeline::defaultPipelineConfigInfo(pipelineConfig);

            pipelineConfig.renderPass = engine.getGraphicsManager().getVkInterface().getSwapChainRenderPass();

            pipelineConfig.pipelineLayout = newData.pipelineLayout;
            newData.pipeline = std::move(std::make_unique<Pipeline>(
                engine,
                device,
                "Engine/Managers/Graphics/DefaultShaders/Compiled/default.vert.spv",
                "Engine/Managers/Graphics/DefaultShaders/Compiled/default.frag.spv",
                pipelineConfig));

            break;
        case gde::VulkanInterface::LIGHT:
            VkPushConstantRange lightPushConstantRange{};
            lightPushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            lightPushConstantRange.offset = 0;
            lightPushConstantRange.size = sizeof(PointLightPushConstantData);

            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
            pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
            pipelineLayoutInfo.pushConstantRangeCount = 1;
            pipelineLayoutInfo.pPushConstantRanges = &lightPushConstantRange;
            if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &newData.pipelineLayout) !=
                VK_SUCCESS) {
                engine.getDebugManager().getLogger().log(Logger::Error, "failed to create pipeline layout!");
            }

            Pipeline::defaultPipelineConfigInfo(pipelineConfig);
            pipelineConfig.attributeDescriptions.clear();
            pipelineConfig.bindingDescriptions.clear();
            pipelineConfig.renderPass = swapChain->getRenderPass();
            pipelineConfig.pipelineLayout = newData.pipelineLayout;
            newData.pipeline = std::make_unique<Pipeline>(
                engine,
                device,
                "Engine/Managers/Graphics/DefaultShaders/Compiled/light.vert.spv",
                "Engine/Managers/Graphics/DefaultShaders/Compiled/light.frag.spv",
                pipelineConfig);
            break;
        }

        pipelines.push_back(std::move(newData));
        return pipelines.size() - 1;
    }

    void VulkanInterface::bindPipeline(int pipelineIndex) {
        pipelines[pipelineIndex].pipeline->bind(commandBuffers[currentFrameIndex]);
    }

    void VulkanInterface::bindDescriptorSets(int pipelineIndex) {
        vkCmdBindDescriptorSets(
            commandBuffers[currentFrameIndex],
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipelines[pipelineIndex].pipelineLayout,
            0,
            1,
            &globalDescriptorSets[currentFrameIndex],
            0,
            nullptr);
    }

    void VulkanInterface::setPushConstantData(int pipelineIndex, PointLightPushConstantData push) {
        vkCmdPushConstants(
            commandBuffers[currentFrameIndex],
            pipelines[pipelineIndex].pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(PointLightPushConstantData),
            &push);
    }

    void VulkanInterface::setPushConstantData(int pipelineIndex, DefaultPushConstantData push) {
        vkCmdPushConstants(
            commandBuffers[currentFrameIndex],
            pipelines[pipelineIndex].pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(DefaultPushConstantData),
            &push);
    }

    void VulkanInterface::drawModel(std::shared_ptr<Model> model) {
        model->bind(commandBuffers[currentFrameIndex]);
        model->draw(commandBuffers[currentFrameIndex]);
    }
    void VulkanInterface::drawQuad() {
        vkCmdDraw(commandBuffers[currentFrameIndex], 6, 1, 0, 0); 
    }
}