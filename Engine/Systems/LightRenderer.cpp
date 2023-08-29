#include "LightRenderer.h"

#include "../GoldDayEngine.h"

namespace gde::system {
	LightRenderer::LightRenderer(GoldDayEngine& _engine)
		: engine{ _engine } {

		pipelineIndex = engine.getGraphicsManager().getVkInterface().createPipeline(VulkanInterface::PipelineType::LIGHT);
	}

	LightRenderer::~LightRenderer() {
	}

	void LightRenderer::renderLights() {
		auto& vkInterface = engine.getGraphicsManager().getVkInterface();

		vkInterface.bindPipeline(pipelineIndex);
		vkInterface.bindDescriptorSets(pipelineIndex);

		for (auto& kv : engine.getGOM().gameObjects) {
			auto& obj = kv.second;
			if (obj.pointLight == nullptr) continue;

			VulkanInterface::PointLightPushConstantData push{};
			push.position = glm::vec4(obj.transform.translation, 1.f);
			push.color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
			push.radius = obj.transform.scale;

			vkInterface.setPushConstantData(pipelineIndex, push);

			vkInterface.drawQuad();
		}
	}
}