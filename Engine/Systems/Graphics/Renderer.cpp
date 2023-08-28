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
	

	Renderer::Renderer(GoldDayEngine& _engine, 
		Device& _device, 
		VkRenderPass renderPass, 
		VkDescriptorSetLayout globalSetLayout)
		: engine{_engine},device{ _device } {
		
		defaultPipelineIndex = engine.getGraphicsManager().getVkInterface().createPipeline(VulkanInterface::PipelineType::DEFAULT);
		lightPipelineIndex = engine.getGraphicsManager().getVkInterface().createPipeline(VulkanInterface::PipelineType::LIGHT);
	}

	Renderer::~Renderer() {
	}


	void Renderer::renderGameObjects() {
		auto& vkInterface = engine.getGraphicsManager().getVkInterface();
		
		vkInterface.bindPipeline(defaultPipelineIndex);
		vkInterface.bindDescriptorSets(defaultPipelineIndex);

		for (auto& kv : engine.gameObjects) {
			auto& obj = kv.second;
			if (obj.model == nullptr) continue;

			VulkanInterface::DefaultPushConstantData push{};
			push.modelMatrix = obj.transform.mat4();

			vkInterface.setPushConstantData(defaultPipelineIndex, push);
			
			vkInterface.drawModel(obj.model);
		}
	}

	void Renderer::renderLights() {
		auto& vkInterface = engine.getGraphicsManager().getVkInterface();

		vkInterface.bindPipeline(lightPipelineIndex);
		vkInterface.bindDescriptorSets(lightPipelineIndex);

		for (auto& kv : engine.gameObjects) {
			auto& obj = kv.second;
			if (obj.pointLight == nullptr) continue;

			VulkanInterface::PointLightPushConstantData push{};
			push.position = glm::vec4(obj.transform.translation, 1.f);
			push.color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
			push.radius = obj.transform.scale;

			vkInterface.setPushConstantData(lightPipelineIndex, push);

			vkInterface.drawQuad();
		}
	}



}