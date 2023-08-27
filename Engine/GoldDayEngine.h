#pragma once

#include <glm/vec2.hpp>

#include "Systems/Debug/DebugManager.h"
#include "Systems/Graphics/GraphicsManager.h"
#include "Systems/HumanInterfaceManager/HumanInterfaceManager.h"
namespace gde {
	
	class GoldDayEngine{

	public:
		GoldDayEngine(std::string windowName, glm::vec2 windowDimentions);
		~GoldDayEngine();

		void start();
		void end();

		system::DebugManager& getDebugManager() { return debugManager; }
		system::GraphicsManager& getGraphicsManager() { return graphicsManager; }
		system::HIM& getHumanInterfaceManager() { return humanInterfaceManager; }
		system::HIM& getHIM() { return humanInterfaceManager; }

		

	private:
		system::DebugManager debugManager;
		system::GraphicsManager graphicsManager;
		system::HIM humanInterfaceManager;

		float deltaTime = 0;

		void loadGameObjectsTEMP();
		void cameraControllerTEMP(float dt, GameObject& gameObject);

	public:
		std::vector<GameObject> gameObjects; // TODO: move to gom
	};

}