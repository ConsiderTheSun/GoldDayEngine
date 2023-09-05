#include "EngineIncludes.h"

#include "CameraUpdate.h"

#include "../GoldDayEngine.h"

namespace gde::system {
	CameraUpdate::CameraUpdate(GoldDayEngine& _engine)
		: System{ _engine } {

	}

	CameraUpdate::~CameraUpdate() {
	}

	void CameraUpdate::setSignature() {
		gde::Signature cameraUpdateSignature;
		cameraUpdateSignature.set(engine.getMOM().getComponentType<component::Transform>());
		cameraUpdateSignature.set(engine.getMOM().getComponentType<component::Camera>());

		systemSignature = cameraUpdateSignature;
	}


	void CameraUpdate::setCameraAspectRatio() {
		GOIDItr itr;
		GOIDItr end;
		engine.getMOM().getRelevantGOIDs(systemSignature, itr, end);
		for (; itr != end; itr++) {
			engine.getMOM().getComponent<component::Camera>(*itr)
				.setPerspectiveProjection(engine.getGraphicsManager().getVkInterface().getAspectRatio(), 0.1f, 100.f);
		}
	}

	void CameraUpdate::setCameraUBOData(GlobalUbo& ubo) {
		
		// update camera data
		GOIDItr itr;
		GOIDItr end;
		engine.getMOM().getRelevantGOIDs(systemSignature, itr, end);
		for (; itr != end; itr++) {
			auto& t = engine.getMOM().getComponent<component::Transform>(*itr);
			auto& cameraComp = engine.getMOM().getComponent<component::Camera>(*itr);
			cameraComp.setViewYXZ(t.translation, t.rotation);

			// passes camera data to ubo
			ubo.projectionView = cameraComp.getProjection() * cameraComp.getView();
			ubo.view = cameraComp.getView();
			ubo.inverseView = cameraComp.getInverseView();

			return; // only checks the first camera component that was made
		}
	}
}