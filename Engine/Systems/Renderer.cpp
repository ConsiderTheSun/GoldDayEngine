#include "EngineIncludes.h"

#include "Renderer.h"

#include "../GoldDayEngine.h"

namespace gde::system {
	

	Renderer::Renderer(GoldDayEngine& _engine)
		: System{_engine} {
		
		pipelineIndex = engine.getGraphicsManager().getVkInterface().createPipeline(VulkanInterface::PipelineType::DEFAULT);
	}

	Renderer::~Renderer() {
	}


	void Renderer::renderGameObjects() {
		auto& vkInterface = engine.getGraphicsManager().getVkInterface();
		
		vkInterface.bindPipeline(pipelineIndex);
		vkInterface.bindDescriptorSets(pipelineIndex);

		for (auto& kv : engine.getMOM().getGOM().gameObjects) {
			GOID id = kv.first;
			if ((engine.getMOM().getSignature(id) & systemSignature) != systemSignature) continue;

			VulkanInterface::DefaultPushConstantData push{};

			component::Transform& transform = engine.getMOM().getComponent<component::Transform>(id);
			push.modelMatrix = transform.mat4();

			vkInterface.setPushConstantData(pipelineIndex, push);
			component::Render& render = engine.getMOM().getComponent<component::Render>(id);
			vkInterface.drawModel(render.model);
		}
	}


}