
#pragma once
#include "GameObjectManager.h"

#include "../GoldDayEngine.h"

#include "../../Components/PointLight.h"
#include "../../Components/Transform.h"

namespace gde::manager {
	template<typename T>
	inline void GOM::addComponent(Entity id, T component) {

		auto itr = gameObjects.find(id);
		if (itr == gameObjects.end()) {
			engine.getDebugManager().getLogger().log(Logger::Channel::Error, "Tried to add component to invalid id");
			return;
		}

		GameObject& go = itr->second;

		if (std::is_same<T, component::PointLight>::value) {
			go.hasPointLight = true;
		}
		else if (std::is_same<T, component::Transform>::value) {
			go.hasTransform = true;
		}

		engine.getComponentManager().addComponent<T>(id, component);
	}

}