#pragma once

#include "ComponentInterface.h"
namespace gde {
	class TransformInterface : public ComponentInterface {
	public:

		TransformInterface(GoldDayEngine& _engine) : ComponentInterface{ _engine } {};
		std::string getType() const override { return typeid(TransformInterface).name(); };

		component::Transform makeComponent(glm::vec3 translation = glm::vec3(), glm::vec3 rotation=glm::vec3(), float scale=1){
			return component::Transform{ translation ,rotation,scale };
		}
	};
}