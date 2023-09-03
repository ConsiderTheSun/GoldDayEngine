#include "EngineIncludes.h"
#include "Light.h"

#include "../GoldDayEngine.h"

#include "../GameObject/GameObject.h"
namespace gde::system {
	Light::Light(GoldDayEngine& _engine)
		: System{ _engine } {

	}

	Light::~Light() {
	}

	void Light::setSignature() {
		gde::Signature lightSignature;
		lightSignature.set(engine.getMOM().getComponentType<component::Transform>());
		lightSignature.set(engine.getMOM().getComponentType<component::PointLight>());

		systemSignature = lightSignature;
	}

	void Light::updateLightData(const Camera& camera) {
		VulkanInterface::GlobalUbo ubo{};

		// update camera stuff
		ubo.projectionView = camera.getProjection() * camera.getView();
		ubo.view = camera.getView();
		ubo.inverseView = camera.getInverseView();


		// update light stuff
		int lightIndex = 0;

		GOIDItr itr;
		GOIDItr end;
		engine.getMOM().getRelevantGOIDs(systemSignature, itr, end);
		for (; itr != end; itr++) {
			GOID id = *itr;


			component::PointLight& pointLight = engine.getMOM().getComponent<component::PointLight>(id);

			assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

			// copy light to ubo
			component::Transform& transform = engine.getMOM().getComponent<component::Transform>(id);
			ubo.pointLights[lightIndex].position = glm::vec4(transform.translation, 1.f);
			ubo.pointLights[lightIndex].color = glm::vec4(pointLight.color, pointLight.lightIntensity);

			lightIndex += 1;
		}
		ubo.numLights = lightIndex;

		// writes to gpu
		engine.getGraphicsManager().getVkInterface().setUboData(ubo);
	}
}