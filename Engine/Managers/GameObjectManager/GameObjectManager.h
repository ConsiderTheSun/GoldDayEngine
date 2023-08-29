#pragma once

#include "../Manager.h"
#include "../../GameObject/GameObject.h"
#include "../../Components/PointLight.h"

namespace gde::manager {
	class GameObjectManager : public Manager {
	public:
		GameObjectManager(GoldDayEngine& _engine);
		~GameObjectManager();

		std::string getType() const override { return typeid(GameObjectManager).name(); };
		GameObject& AddGameObject();

		void addComponent(Entity id, component::PointLight component);
	//private:
		GameObject::Map gameObjects;
		
		id_t currentId = 0;
	};

	typedef GameObjectManager GOM;
}