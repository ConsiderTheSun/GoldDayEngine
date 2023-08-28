#pragma once

#include <glm/vec2.hpp>
#include <chrono>

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

		// TODO: move to dedicated frame rate manager class
		std::chrono::steady_clock::time_point currentTime;
		float getDeltaTime();

		void loadGameObjectsTEMP();
		void customControllerTEMP(float dt, GameObject& gameObject);

	public:
		GameObject::Map gameObjects; // TODO: move to gom
	};

}