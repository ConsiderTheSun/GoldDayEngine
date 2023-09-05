#pragma once

#include "ComponentInterface.h"
namespace gde {
	class PointLightInterface : public ComponentInterface {
	public:
		PointLightInterface(GoldDayEngine& _engine) : ComponentInterface{ _engine } {};
		std::string getType() const override { return typeid(PointLightInterface).name(); };


		component::PointLight makeComponent(float lightIntensity = 1.0f, glm::vec3 color=glm::vec3()) {
			return component::PointLight{ lightIntensity, color };
		}
	};
}