#include "EngineIncludes.h"
#include "GameObjectManager.h"
#include <iostream>
#include <type_traits>

#include "../GoldDayEngine.h"
#include <memory>
namespace gde::manager {
	GOM::GameObjectManager(GoldDayEngine& _engine) :Manager(_engine) {
	}
	GOM::~GameObjectManager() {

	}

	GameObject& GOM::AddGameObject() {

		auto a = gameObjects.emplace(currentId, GameObject{ currentId });
		++currentId;
		return a.first->second;
	}

	
}