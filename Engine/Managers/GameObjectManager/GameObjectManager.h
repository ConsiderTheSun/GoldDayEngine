#pragma once

#include "../Manager.h"
#include "../../GameObject/GameObject.h"

namespace gde {
	using GOIDContainer = std::unordered_set<GOID>;
	using GOIDItr = GOIDContainer::const_iterator;
}
namespace gde::manager {
	

	class GameObjectManager : public Manager {
	public:
		GameObjectManager(GoldDayEngine& _engine);
		~GameObjectManager();

		std::string getType() const override { return typeid(GameObjectManager).name(); };
		const GOID& addGameObject();

		GameObject& getGameObject(GOID id);
		void getAllGOIDS(GOIDItr& begin, GOIDItr& end);

		template<typename T> 
		inline void addComponent(GOID goID, ComponentType type);
	private:
		

		std::unordered_map<GOID, GameObject> gameObjects;

		GOIDContainer allActiveGOIDs;
		
		GOID currentId = 0;
	};

	typedef GameObjectManager GOM;
}