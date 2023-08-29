#pragma once
#include "Device.h"
#include "../../GameObject/GameObject.h"
#include "Pipeline.h"
#include "SwapChain.h"
#include "../Window.h"
#include "DescriptorSet.h"

#include <memory>
#include <vector>
#include <map>

namespace gde {
	class GoldDayEngine;

	class VulkanInterface {
	public:
#define MAX_LIGHTS 10

		struct PointLight {
			glm::vec4 position{};  // ignore w
			glm::vec4 color{};     // w is intensity
		};

		struct GlobalUbo { // TODO: move maybe?
			glm::mat4 projectionView{ 1.f };
			glm::mat4 view{ 1.f };
			glm::mat4 inverseView{ 1.f };
			glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };  // w is intensity
			PointLight pointLights[MAX_LIGHTS];
			int numLights;
		};

		struct DefaultPushConstantData {
			glm::mat4 modelMatrix{ 1.f };
		};

		struct PointLightPushConstantData {
			glm::vec4 position{};
			glm::vec4 color{};
			float radius;
		};

		enum PipelineType {
			DEFAULT,
			LIGHT
		};

		VulkanInterface(GoldDayEngine& _engine, Window& _window);
		~VulkanInterface();

		VulkanInterface(const VulkanInterface&) = delete;
		VulkanInterface& operator=(const VulkanInterface&) = delete;
	
		void waitIdle() { vkDeviceWaitIdle(device.device()); }

		VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); }
		float getAspectRatio() const { return swapChain->extentAspectRatio(); }
		
		bool isFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}

		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}


	
		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

		Device& getDevice() { return device; }

		VkDescriptorSetLayout getGlobalDescriptorSetLayout() { return globalSetLayout->getDescriptorSetLayout(); }
		VkDescriptorSet getGlobalDescriptorSet(int i) { return globalDescriptorSets[i]; }
		Buffer& getUboBuffer(int i) { return *uboBuffers[i]; }

		void setUboData(GlobalUbo& ubo);

		int createPipeline(PipelineType pipelineType);

		void bindPipeline(int pipelineIndex);
		void bindDescriptorSets(int pipelineIndex);
		void setPushConstantData(int pipelineIndex, DefaultPushConstantData push);
		void setPushConstantData(int pipelineIndex, PointLightPushConstantData push);
		
		void drawModel(std::shared_ptr<Model> model);
		void drawQuad();
	private:

		struct PipelineData {
			std::unique_ptr<Pipeline> pipeline;
			VkPipelineLayout pipelineLayout{};
		};

		std::vector<PipelineData> pipelines;

		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		void createUbo();

		void createDescriptorSets();

		GoldDayEngine& engine;
		Window& window;
		
		Device device{ engine, window };

		std::unique_ptr<SwapChain> swapChain;

		std::vector<VkCommandBuffer> commandBuffers;


		std::vector<std::unique_ptr<Buffer>> uboBuffers;
		std::unique_ptr<DescriptorPool> globalPool;
		std::vector<VkDescriptorSet> globalDescriptorSets; 

		std::unique_ptr<gde::DescriptorSetLayout> globalSetLayout;
		
		uint32_t currentImageIndex;
		int currentFrameIndex{ 0 };

		bool isFrameStarted{false};
	};
}