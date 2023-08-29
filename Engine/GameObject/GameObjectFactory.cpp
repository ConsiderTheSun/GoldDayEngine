#include "GameObjectFactory.h"

namespace gde {
	GOF::GameObjectFactory(manager::GOM& _gom) : gom{ _gom } {

	}

	GameObject& GOF::makeEmptyGameObject() {
		 return gom.AddGameObject();
	}
}