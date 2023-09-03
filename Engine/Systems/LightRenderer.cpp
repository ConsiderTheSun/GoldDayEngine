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

	void LightRenderer::renderLights() {
		auto& vkInterface = engine.getGraphicsManager().getVkInterface();

		vkInterface.bindPipeline(pipelineIndex);
		vkInterface.bindDescriptorSets(pipelineIndex);

		for (auto& kv : engine.getMOM().getGOM().gameObjects) {
			GOID id = kv.first;

			if ((engine.getMOM().getSignature(id) & systemSignature) != systemSignature) continue;

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