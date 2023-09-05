#include "GameObjectInterface.h"

namespace gde {
	template<typename T>
	inline void GOInterface::addComponent(GOID goID, const T& component) {
		engine.getMOM().addComponent<T>(goID, component);
	}
}