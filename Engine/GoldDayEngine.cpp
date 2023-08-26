
#include "GoldDayEngine.h"
namespace gde {

	GoldDayEngine::GoldDayEngine(std::string windowName, glm::vec2 windowDimentions) :
			debugManager(*this), 
			graphicsManager(*this, windowName, windowDimentions){
		debugManager.getLogger().Log(Logger::Verbose, "GoldDayEngine Instantiated");
	}

	GoldDayEngine::~GoldDayEngine(){
		debugManager.getLogger().Log(Logger::Verbose, "GoldDayEngine Freed");
	}

	void GoldDayEngine::Start() {
		debugManager.getLogger().Log(Logger::Info, "GoldDayEngine Starting");

		while (!graphicsManager.window.shouldClose()) {
			graphicsManager.window.pollEvents();
			graphicsManager.drawFrame();
		}

		graphicsManager.waitIdle();
	}

}