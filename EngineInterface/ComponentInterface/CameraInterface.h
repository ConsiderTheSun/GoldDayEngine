#pragma once

#include "ComponentInterface.h"
namespace gde {
	class CameraInterface : public ComponentInterface {
	public:

		CameraInterface(GoldDayEngine& _engine) : ComponentInterface{ _engine } {};
		std::string getType() const override { return typeid(CameraInterface).name(); };

		GOID mainCamera() { 
			gde::GOIDItr begin;
			gde::GOIDItr end;
			gde::Signature cameraUpdateSignature = engine.getMOM().getSystem<system::CameraUpdate>()->getSignature();
			engine.getMOM().getRelevantGOIDs(cameraUpdateSignature, begin, end);

			return *begin;
		}

		float getFovY() {
			return engine.getMOM().getComponent<component::Camera>(mainCamera()).fovy;
		}
		void setFovY(float newFovy) {
			engine.getMOM().getComponent<component::Camera>(mainCamera()).fovy = newFovy;
			engine.getMOM().getComponent<component::Camera>(mainCamera())
				.setPerspectiveProjection(engine.getGraphicsManager().getVkInterface().getAspectRatio(), 0.1f, 100.f);

			engine.getMOM().getSystem<system::CameraUpdate>()->setCameraAspectRatio();
		}
	};
}