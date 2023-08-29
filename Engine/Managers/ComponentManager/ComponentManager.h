#pragma once
#include "ComponentArray.h"

#include "../../Components/PointLight.h"

namespace gde::manager {

	class ComponentManager {
	public:

		//template<typename T>
		void addComponent(Entity entity, component::PointLight component)
		{
			// Add a component to the array for an entity
			pointLightArray.InsertData(entity, component);

		}

		//template<typename T>
		component::PointLight& getComponent(Entity entity)
		{
			// Get a reference to a component from the array for an entity
			return pointLightArray.GetData(entity);
		}
	private:
		ComponentArray pointLightArray;
	};
}