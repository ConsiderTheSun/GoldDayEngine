
#pragma once
#include "GameObjectManager.h"

#include "../GoldDayEngine.h"

#include "../../Components/Transform.h"
#include "../../Components/Render.h"
#include "../../Components/PointLight.h"


namespace gde::manager {
	template<typename T>
	inline void GOM::addComponent(GOID goID, ComponentType type) {

		auto itr = gameObjects.find(goID);
		if (itr == gameObjects.end()) {
			engine.getDebugManager().getLogger().log(Logger::Channel::Error, "Tried to add component to invalid id");
			return;
		}

		GameObject& go = itr->second;

		go.signature.set(type, true);
	}

}