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

namespace gde::system {
	

	Renderer::Renderer(GoldDayEngine& _engine)
		: engine{_engine} {
		
		pipelineIndex = engine.getGraphicsManager().getVkInterface().createPipeline(VulkanInterface::PipelineType::DEFAULT);
	}

	Renderer::~Renderer() {
	}


	void Renderer::renderGameObjects() {
		auto& vkInterface = engine.getGraphicsManager().getVkInterface();
		
		vkInterface.bindPipeline(pipelineIndex);
		vkInterface.bindDescriptorSets(pipelineIndex);

		for (auto& kv : engine.getGOM().gameObjects) {
			auto& obj = kv.second;
			if (obj.model == nullptr) continue;

			VulkanInterface::DefaultPushConstantData push{};
			push.modelMatrix = obj.transform.mat4();

			vkInterface.setPushConstantData(pipelineIndex, push);
			
			vkInterface.drawModel(obj.model);
		}
	}


}