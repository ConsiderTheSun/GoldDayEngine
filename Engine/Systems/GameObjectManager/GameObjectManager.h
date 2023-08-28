#pragma once

#include "../System.h"
#include "../../GameObject/GameObject.h"

namespace gde::system {
	class GameObjectManager : public System {
	public:
		GameObjectManager(GoldDayEngine& _engine);
		~GameObjectManager();

	private:
		GameObject::Map gameObjects;
		
	};

	typedef GameObjectManager GOM;
}