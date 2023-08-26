#pragma once
#include "Device.h"
#include "../../GameObject.h"
#include "Pipeline.h"
#include "SwapChain.h"
#include "../Window.h"

#include <memory>
#include <vector>

namespace gde {
	class GoldDayEngine;

	class VulkanInterface {
	public:
		VulkanInterface(GoldDayEngine& e, Window& w);
		~VulkanInterface();

		VulkanInterface(const VulkanInterface&) = delete;
		VulkanInterface& operator=(const VulkanInterface&) = delete;
	
		void waitIdle() { vkDeviceWaitIdle(device.device()); }
		void drawFrame();
	private:
		void loadGameObjects();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void renderGameObjects(VkCommandBuffer commandBuffer);

		GoldDayEngine& engine;
		Window& window;
		
		Device device{ engine, window };

		std::unique_ptr<SwapChain> swapChain;
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<GameObject> gameObjects;
	};
}