#pragma once
#include "ComponentArray.h"

#include <memory>
#include <any>

#include "../../Components/PointLight.h"
#include "../../Components/Transform.h"

namespace gde::manager {

	class ComponentManager {
	public:

		template<typename T>
		void addComponent(Entity entity, T component)
		{
			bool why = std::is_same<T, component::PointLight>::value;
			// Add a component to the array for an entity
			if (why) {
				//ComponentArray<T> p = std::static_cast<ComponentArray<T>>(pointLightArray);
				pointLightArray.InsertData(entity, component);
			}
			//else if(std::is_same<T, component::Transform>::value) {
			//	//component::Transform _compy = component;
			//	//transformArray.InsertData(entity, _compy);
			//}

		}

		//template<typename T>
		component::PointLight& getComponent(Entity entity)
		{
			// Get a reference to a component from the array for an entity
			return pointLightArray.GetData(entity);
		}
	private:
		ComponentArray<component::PointLight> pointLightArray;
		ComponentArray<component::Transform> transformArray;
	};
}