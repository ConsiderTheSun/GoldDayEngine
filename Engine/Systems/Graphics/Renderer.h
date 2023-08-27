#pragma once

#include "VulkanInterface/Device.h"
#include "../GameObject.h"
#include "VulkanInterface/Pipeline.h"
#include "Camera.h"

// std
#include <memory>
#include <vector>

namespace gde {
	class GoldDayEngine;

	class Renderer {
	public:
		Renderer(GoldDayEngine& _engine, Device& _device, VkRenderPass renderPass);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, 
			std::vector<GameObject>& gameObjects,
			const Camera& camera);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		Device& device;
		GoldDayEngine& engine;

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}