#pragma once

#include "../GoldDayEngine.h"

#include "Application.h"

namespace gde {
	class EngineInterface {
	public:
		EngineInterface(const std::string& windowName, glm::vec2 windowDimentions) 
			:engine{ windowName, windowDimentions } {

		}

		EngineInterface(const EngineInterface&) = delete;
		EngineInterface& operator=(const EngineInterface&) = delete;

		// TODO: remove when scripting is added
		void setApp(Application* appPtr) {
			engine.appPtr = appPtr;
		};

		void start() { engine.start(); }


		GoldDayEngine& getEngine() { return engine; } // TODO: replace with robust interface 

	private:
		GoldDayEngine engine;
	};
}