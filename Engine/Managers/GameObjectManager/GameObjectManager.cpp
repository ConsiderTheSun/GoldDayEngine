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

	GameObject& GOM::addGameObject() {
		auto a = gameObjects.emplace(currentId, GameObject{ currentId });
		++currentId;
		return a.first->second;
	}


	GameObject& GOM::getGameObject(GOID id) {
		assert(gameObjects.find(id) != gameObjects.end() && "invalid gameObject ID searched for.");
		return gameObjects.find(id)->second;
	}


	
}