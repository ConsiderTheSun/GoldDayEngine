#include "EngineIncludes.h"
#include "DebugManager.h"
#include "../GoldDayEngine.h"

namespace gde::manager {
	DebugManager::DebugManager(GoldDayEngine& _engine) : Manager(_engine) {
		logger.log(Logger::Verbose, "DebugManager Instantiated");
	};

	DebugManager::~DebugManager() {
		logger.log(Logger::Verbose, "DebugManager Freed");
	};

	bool DebugManager::isLightRenderingEnabled() {
		return lightRenderingEnabled; 
	}

	void DebugManager::setLightRendering(bool _renderLights) { 
		lightRenderingEnabled = _renderLights; 
		logger.log(Logger::Info, "Light Rendering " + std::string(_renderLights ? "Enabled" : "Disabled"));
	}

}