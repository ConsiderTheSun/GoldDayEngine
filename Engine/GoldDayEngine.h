#pragma once

#include "Managers/Graphics/GraphicsManager.h"
#include "Managers/HumanInterfaceManager/HumanInterfaceManager.h"

#include "Managers/MetaObjectManager/MetaObjectManager.h"
#include "Managers/MetaObjectManager/MetaObjectManager.inl"

class Application; // TODO: remove this

namespace gde {
	
	class GoldDayEngine{

	public:
		GoldDayEngine(const std::string& windowName, glm::vec2 windowDimentions);
		~GoldDayEngine();

		void start();
		void end();

		manager::DebugManager& getDebugManager() { return debugManager; }
		manager::GraphicsManager& getGraphicsManager() { return graphicsManager; }
		manager::HIM& getHIM() { return humanInterfaceManager; }
		manager::MOM& getMOM() { return metaObjectManager; }

		Application* appPtr; // TODO: remove this when scripting is done

	private:
		manager::DebugManager debugManager;
		manager::GraphicsManager graphicsManager;
		manager::HIM humanInterfaceManager;

		manager::MOM metaObjectManager;

		// TODO: move to dedicated frame rate manager system
		std::chrono::steady_clock::time_point currentTime;
		float getDeltaTime();

		
	};

}