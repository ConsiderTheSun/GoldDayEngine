#pragma once

#include "Managers/Graphics/GraphicsManager.h"
#include "Managers/HumanInterfaceManager/HumanInterfaceManager.h"
#include "Managers/GameObjectManager/GameObjectManager.h"
#include "Managers/ComponentManager/ComponentManager.h"
#include "GameObject/GameObjectFactory.h"
namespace gde {
	
	class GoldDayEngine{

	public:
		GoldDayEngine(std::string windowName, glm::vec2 windowDimentions);
		~GoldDayEngine();

		void start();
		void end();

		manager::DebugManager& getDebugManager() { return debugManager; }
		manager::GraphicsManager& getGraphicsManager() { return graphicsManager; }
		manager::HIM& getHumanInterfaceManager() { return humanInterfaceManager; }
		manager::HIM& getHIM() { return humanInterfaceManager; }
		manager::GOM& getGameObjectManager() { return gom; }
		manager::GOM& getGOM() { return gom; }
		manager::ComponentManager& getComponentManager() { return componentManager; }

	private:
		manager::DebugManager debugManager;
		manager::GraphicsManager graphicsManager;
		manager::HIM humanInterfaceManager;
		manager::GOM gom;
		manager::ComponentManager componentManager;

		GOF gof;

		// TODO: move to dedicated frame rate manager system
		std::chrono::steady_clock::time_point currentTime;
		float getDeltaTime();

		void loadGameObjectsTEMP();
		void customControllerTEMP(float dt, GameObject& gameObject);
	};

}