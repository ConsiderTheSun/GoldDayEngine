#include "GraphicsManager.h"
#include "../GoldDayEngine.h"

namespace gde::system {
	GraphicsManager::GraphicsManager(GoldDayEngine& e, std::string windowName, glm::vec2 windowDimentions) :
		System(e), window(e, windowName, windowDimentions) {
		engine.getDebugManager().getLogger().Log(Logger::Verbose, "GraphicsManager Instantiated");
	}

	GraphicsManager::~GraphicsManager() {
		engine.getDebugManager().getLogger().Log(Logger::Verbose, "GraphicsManager Freed");
	}

	void GraphicsManager::drawFrame() {
		vkInterface.drawFrame();
	}

}