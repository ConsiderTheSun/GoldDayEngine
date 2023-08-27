#include "Renderer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

#include "../GoldDayEngine.h"

namespace gde {
	struct SimplePushConstantData {
		glm::mat4 transform{ 1.f };
		glm::mat4 modelMatrix{ 1.f };
	};

	Renderer::Renderer(GoldDayEngine& _engine, Device& _device, VkRenderPass renderPass)
		: engine{_engine},device{ _device } {
		createPipelineLayout();
		createPipeline(renderPass);
	}

	Renderer::~Renderer() {
		vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
	}


	void Renderer::createPipelineLayout() {
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS) {
			engine.getDebugManager().getLogger().log(Logger::Error, "failed to create pipeline layout!");
		}
	}
	void Renderer::createPipeline(VkRenderPass renderPass) {
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		Pipeline::defaultPipelineConfigInfo(pipelineConfig);
		
		pipelineConfig.renderPass = engine.getGraphicsManager().getVkInterface().getSwapChainRenderPass();

		pipelineConfig.pipelineLayout = pipelineLayout;
		pipeline = std::make_unique<Pipeline>(
			engine,
			device,
			"Engine/Systems/Graphics/DefaultShaders/Compiled/default.vert.spv",
			"Engine/Systems/Graphics/DefaultShaders/Compiled/default.frag.spv",
			pipelineConfig);
	}

	void Renderer::renderGameObjects(VkCommandBuffer commandBuffer, 
		std::vector<GameObject>& gameObjects, 
		const Camera& camera) {
		pipeline->bind(commandBuffer);

		auto projectionView = camera.getProjection() * camera.getView();

		for (auto& obj : gameObjects) {
			
			SimplePushConstantData push{};
			auto modelMatrix = obj.transform.mat4();

			
			push.transform = projectionView * modelMatrix;
			push.modelMatrix = modelMatrix;

			vkCmdPushConstants(
				commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);
			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}

	}


}