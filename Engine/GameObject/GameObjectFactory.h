#pragma once
#include "GameObject.h"
#include "../Managers/GameObjectManager/GameObjectManager.h"

namespace gde {
	class GameObjectFactory {
	public:
		GameObjectFactory(manager::GOM& _gom);

		GameObject& makeEmptyGameObject();
	private:
		manager::GOM& gom;
	};

	typedef GameObjectFactory GOF;

}