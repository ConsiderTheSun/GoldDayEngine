#include "EngineIncludes.h"
#include "LightRenderer.h"

#include "../GoldDayEngine.h"

namespace gde::system {
	LightRenderer::LightRenderer(GoldDayEngine& _engine)
		: System{ _engine } {

		pipelineIndex = engine.getGraphicsManager().getVkInterface().createPipeline(VulkanInterface::PipelineType::LIGHT);
	}

	LightRenderer::~LightRenderer() {
	}

	void LightRenderer::setSignature() {
		gde::Signature lightRendererSignature;
		lightRendererSignature.set(engine.getMOM().getComponentType<component::Transform>());
		lightRendererSignature.set(engine.getMOM().getComponentType<component::PointLight>());

		systemSignature = lightRendererSignature;
	}

	void LightRenderer::renderLights() {
		auto& vkInterface = engine.getGraphicsManager().getVkInterface();

		vkInterface.bindPipeline(pipelineIndex);
		vkInterface.bindDescriptorSets(pipelineIndex);

		GOIDItr itr;
		GOIDItr end;
		engine.getMOM().getRelevantGOIDs(systemSignature, itr, end);
		for (; itr != end; itr++) {
			GOID id = *itr;

			component::Transform& transform = engine.getMOM().getComponent<component::Transform>(id);
			component::PointLight& pointLight = engine.getMOM().getComponent<component::PointLight>(id);

			VulkanInterface::PointLightPushConstantData push{};
			push.position = glm::vec4(transform.translation, 1.f);
			push.color = glm::vec4(pointLight.color, pointLight.lightIntensity);
			push.radius = transform.scale;

			vkInterface.setPushConstantData(pipelineIndex, push);

			vkInterface.drawQuad();
		}
	}
}