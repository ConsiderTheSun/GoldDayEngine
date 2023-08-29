#include "GameObjectManager.h"
#include <iostream>
#include <type_traits>

namespace gde::manager {
	GOM::GameObjectManager(GoldDayEngine& _engine) :Manager(_engine) {
	}
	GOM::~GameObjectManager() {

	}

	GameObject& GOM::AddGameObject() {

		auto a = gameObjects.emplace(currentId, GameObject{ currentId++ });
		return a.first->second;
	}

	void GOM::addComponent(Entity id, component::PointLight component) {
		//engine.getComponentManager().addComponent(id, component);
		//gameObjects[id].hasPointLight = true;
	}
}