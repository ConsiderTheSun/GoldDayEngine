#pragma once
#include "GameObject.h"
#include "../Managers/GameObjectManager/GameObjectManager.h"

namespace gde {
	class GameObjectFactory {
	public:
		GameObjectFactory();

		// TODO: GameObject& makeFromArchetype(const std::string& archName, GameObject& go);
		//		 bool createArchetype(const std::string& archName, const GameObject& templateGO);
	private:
	};

	typedef GameObjectFactory GOF;

}