#pragma once

#include "ComponentInterface.h"
namespace gde {
	class CameraInterface : public ComponentInterface {
	public:

		CameraInterface(GoldDayEngine& _engine) : ComponentInterface{ _engine } {};
		std::string getType() const override { return typeid(CameraInterface).name(); };

		component::Camera makeComponent() {
			return component::Camera{  }; // TODO: fill in
		}

		float getFovY() {
			return engine.getGraphicsManager().mainCamera.fovy;
		}
		void setFovY(float newFovy) {
			engine.getGraphicsManager().mainCamera.fovy = newFovy;
			engine.getGraphicsManager().setCameraAspectRatio(engine.getGraphicsManager().getVkInterface().getAspectRatio());
		}

		void setViewYXZ(glm::vec3 position, glm::vec3 rotation) {
			engine.getGraphicsManager().mainCamera.setViewYXZ(position,rotation);
		}
	};
}