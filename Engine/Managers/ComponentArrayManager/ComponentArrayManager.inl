#include "ComponentArrayManager.h"

namespace gde::manager {


	template<typename T>
	inline void CAM::registerComponent()
	{
		const char* typeName = typeid(T).name();

		assert(componentArrays.find(typeName) == componentArrays.end() && "Registering component type more than once");

		// Add this component type to the component type map
		componentTypes.insert({ typeName, componentArrays.size()});

		// Create a ComponentArray pointer and add it to the component arrays map
		componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

	}

	template<typename T>
	inline std::shared_ptr< ComponentArray<T>> CAM::getComponentArray() {
		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeid(T).name()]);
	}

	template<typename T>
	inline void CAM::addComponent(GOID goID, const T& component){
		getComponentArray<T>()->InsertData(goID, component);

	}

	template<typename T>
	inline T& CAM::getComponent(GOID goID){
		// Get a reference to a component from the array for an entity
		return getComponentArray<T>()->GetData(goID);
	}

	template<typename T>
	inline ComponentType CAM::getComponentType() {
		assert(componentTypes.find(typeid(T).name()) != componentTypes.end() && "tried to get unregistered component type");
		return componentTypes[typeid(T).name()];
	}
}
