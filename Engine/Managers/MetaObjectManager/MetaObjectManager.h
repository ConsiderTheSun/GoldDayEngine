#pragma once

#include "Managers/SystemUpkeepManager/SystemUpkeepManager.h"
#include "Managers/SystemUpkeepManager/SystemUpkeepManager.inl"

#include "Managers/GameObjectManager/GameObjectManager.h"

#include "Managers/ComponentArrayManager/ComponentArrayManager.h"
#include "Managers/ComponentArrayManager/ComponentArrayManager.inl"

#include "GameObject/GameObjectFactory.h"

namespace gde::manager {

	class MetaObjectManager : public Manager {
	public:
		MetaObjectManager(GoldDayEngine& _engine);
		~MetaObjectManager();

		std::string getType() const override { return typeid(MetaObjectManager).name(); };

		GOM& getGOM() { return gom; } // TODO: remove this

		GameObject& makeGameObject();

		const Signature& getSignature(GOID goID) { return gom.getGameObject(goID).signature; };

		template<typename T>
		void registerSystem(Signature signature) { sum.registerSystem<T>(signature); }

		template<typename T>
		std::shared_ptr<T> getSystem() { return sum.getSystem<T>(); }

		template<typename T>
		void registerComponent() { cam.registerComponent<T>(); }

		template<typename T>
		inline void addComponent(GOID goID, const T& component);

		template<typename T>
		T& getComponent(GOID goID) { return cam.getComponent<T>(goID); }

		template<typename T>
		ComponentType getComponentType() {return cam.getComponentType<T>(); } //TODO: remove this
	private:
		SUM sum;
		GOM gom;
		CAM cam;

	};

	typedef MetaObjectManager MOM;
}