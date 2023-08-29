#include "Light.h"

#include "../GoldDayEngine.h"

#include "../GameObject/GameObject.h"
namespace gde::system {
	Light::Light(GoldDayEngine& _engine)
		: engine{ _engine } {

	}

	Light::~Light() {
	}

	void Light::updateLightData(const Camera& camera) {
		VulkanInterface::GlobalUbo ubo{};

		// update camera stuff
		ubo.projectionView = camera.getProjection() * camera.getView();
		ubo.view = camera.getView();
		ubo.inverseView = camera.getInverseView();


		// update light stuff
		int lightIndex = 0;
		for (auto& kv : engine.getGOM().gameObjects) {
			auto& obj = kv.second;
			if (obj.pointLight == nullptr) continue;

			component::PointLight& pl = engine.getComponentManager().getComponent(obj.getId());



			assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

			// copy light to ubo
			ubo.pointLights[lightIndex].position = glm::vec4(obj.transform.translation, 1.f);
			ubo.pointLights[lightIndex].color = glm::vec4(obj.color, pl.lightIntensity);

			lightIndex += 1;
		}
		ubo.numLights = lightIndex;

		// writes to gpu
		engine.getGraphicsManager().getVkInterface().setUboData(ubo);
	}
}