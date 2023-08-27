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
		
	private:

		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		GoldDayEngine& engine;
		Window& window;
		
		Device device{ engine, window };

		std::unique_ptr<SwapChain> swapChain;

		std::vector<VkCommandBuffer> commandBuffers;
		
		uint32_t currentImageIndex;
		int currentFrameIndex{ 0 };

		bool isFrameStarted{false};
	};
}