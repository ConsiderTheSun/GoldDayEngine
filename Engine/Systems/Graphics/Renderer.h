#pragma once

#include "VulkanInterface/Device.h"
#include "../GameObject/GameObject.h"
#include "VulkanInterface/Pipeline.h"
#include "Camera.h"

// std
#include <memory>
#include <vector>

namespace gde {
	class GoldDayEngine;

	class Renderer {
	public:
		Renderer(GoldDayEngine& _engine, 
			Device& _device, 
			VkRenderPass renderPass, 
			VkDescriptorSetLayout globalSetLayout);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		void renderGameObjects();
		void renderLights();
	private:
		Device& device;
		GoldDayEngine& engine;

		int defaultPipelineIndex;
		int lightPipelineIndex;
	};
}