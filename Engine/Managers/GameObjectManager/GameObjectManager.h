#pragma once

#include "../Manager.h"
#include "../../GameObject/GameObject.h"


namespace gde::manager {
	using ComponentType = std::uint8_t;
	class GameObjectManager : public Manager {
	public:
		GameObjectManager(GoldDayEngine& _engine);
		~GameObjectManager();

		std::string getType() const override { return typeid(GameObjectManager).name(); };
		GameObject& addGameObject();

		GameObject& getGameObject(GOID id);


		template<typename T> 
		inline void addComponent(GOID goID, ComponentType type);
	//private: // TODO: make this private
		std::unordered_map<GOID, GameObject> gameObjects;
		
		GOID currentId = 0;
	};

	typedef GameObjectManager GOM;
}