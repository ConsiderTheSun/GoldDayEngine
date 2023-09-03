#include "MetaObjectManager.h"

namespace gde::manager {
	template<typename T>
	inline void MOM::addComponent(GOID goID, const T& component) { 
		gom.addComponent<T>(goID, cam.getComponentType<T>()); 
		cam.addComponent<T>(goID, component);
	}
}