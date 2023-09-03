#pragma once

#include "Managers/Graphics/GraphicsManager.h"
#include "Managers/HumanInterfaceManager/HumanInterfaceManager.h"

#include "Managers/MetaObjectManager/MetaObjectManager.h"
#include "Managers/MetaObjectManager/MetaObjectManager.inl"

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
		manager::MOM& getMOM() { return metaObjectManager; }

	private:
		manager::DebugManager debugManager;
		manager::GraphicsManager graphicsManager;
		manager::HIM humanInterfaceManager;

		manager::MOM metaObjectManager;

		// TODO: move to dedicated frame rate manager system
		std::chrono::steady_clock::time_point currentTime;
		float getDeltaTime();

		void loadGameObjectsTEMP();
		void customControllerTEMP(float dt, GameObject& gameObject);
	};

}