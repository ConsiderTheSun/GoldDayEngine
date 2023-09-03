#pragma once
#include "ComponentArray.h"

#include <memory>
#include <any>

#include "../../Components/Transform.h"
#include "../../Components/Render.h"
#include "../../Components/PointLight.h"

namespace gde {
	using ComponentType = std::uint8_t;
}

namespace gde::manager {
	class ComponentArrayManager : public Manager {
	public:
		ComponentArrayManager(GoldDayEngine& _engine);
		std::string getType() const override { return typeid(ComponentArrayManager).name(); };

		template<typename T>
		inline void registerComponent();

		template<typename T>
		inline std::shared_ptr< ComponentArray<T>> getComponentArray();

		template<typename T>
		inline void addComponent(GOID goID, const T& component);

		template<typename T>
		inline T& getComponent(GOID goID);

		template<typename T>
		inline ComponentType getComponentType();
	private:
		std::map<const char*, std::shared_ptr<AbstractComponentArray>> componentArrays{};

		std::unordered_map<const char*, ComponentType> componentTypes{};


	};

	typedef ComponentArrayManager CAM;
}