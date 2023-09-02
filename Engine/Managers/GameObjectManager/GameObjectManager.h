#pragma once

#include "../Manager.h"
#include "../../GameObject/GameObject.h"


namespace gde::manager {
	class GameObjectManager : public Manager {
	public:
		GameObjectManager(GoldDayEngine& _engine);
		~GameObjectManager();

		std::string getType() const override { return typeid(GameObjectManager).name(); };
		GameObject& AddGameObject();

		template<typename T> 
		inline void addComponent(Entity id, T component);
	//private:
		GameObject::Map gameObjects;
		
		id_t currentId = 0;
	};

	typedef GameObjectManager GOM;
}