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

		// System Upkeep Manager MetaManaging
		template<typename T>
		void registerSystem() { sum.registerSystem<T>(); }

		template<typename T>
		std::shared_ptr<T> getSystem() { return sum.getSystem<T>(); }

		// Game Object Manager MetaManaging 

		const GOID& makeGameObject();

		const Signature& getSignature(GOID goID) { return gom.getGameObject(goID).getSignature(); };

		void registerRelevantGOIDsTracking(Signature signature) { sum.registerRelevantGOIDsTracking(signature); }
		void getRelevantGOIDs(Signature signature, GOIDItr& begin, GOIDItr& end) {
			return sum.getRelevantGOIDs(signature, begin, end);
		}

		void getAllGOIDS(GOIDItr& begin, GOIDItr& end) {
			return gom.getAllGOIDS( begin, end);
		}



		// Component Array Manager MetaManaging
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