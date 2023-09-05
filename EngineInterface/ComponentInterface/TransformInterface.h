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

		glm::vec3 getPosition(GOID id) {
			return engine.getMOM().getComponent<component::Transform>(id).translation;
		}
		void setPosition(GOID id, glm::vec3 position) {
			engine.getMOM().getComponent<component::Transform>(id).translation = position;
		}

		glm::vec3 getRotation(GOID id) {
			return engine.getMOM().getComponent<component::Transform>(id).rotation;
		}

		void setRotation(GOID id, glm::vec3 rotation) {
			engine.getMOM().getComponent<component::Transform>(id).rotation = rotation;
		}


	};
}