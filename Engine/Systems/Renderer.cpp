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

	void Renderer::setSignature() {
		gde::Signature rendererSignature;
		rendererSignature.set(engine.getMOM().getComponentType<component::Transform>());
		rendererSignature.set(engine.getMOM().getComponentType<component::Render>());

		systemSignature = rendererSignature;
	}

	void Renderer::renderGameObjects() {
		auto& vkInterface = engine.getGraphicsManager().getVkInterface();
		
		vkInterface.bindPipeline(pipelineIndex);
		vkInterface.bindDescriptorSets(pipelineIndex);

		GOIDItr itr;
		GOIDItr end;
		engine.getMOM().getRelevantGOIDs(systemSignature, itr, end);
		for (; itr != end; itr++) {
			GOID id = *itr;

			VulkanInterface::DefaultPushConstantData push{};

			component::Transform& transform = engine.getMOM().getComponent<component::Transform>(id);
			push.modelMatrix = transform.mat4();

			vkInterface.setPushConstantData(pipelineIndex, push);
			component::Render& render = engine.getMOM().getComponent<component::Render>(id);
			vkInterface.drawModel(render.model);
		}
	}


}