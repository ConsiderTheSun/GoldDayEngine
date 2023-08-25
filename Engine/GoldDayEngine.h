#pragma once

#include <glm/vec2.hpp>

#include "Systems/Debug/DebugManager.h"
#include "Systems/Graphics/GraphicsManager.h"
namespace gde {
	
	class GoldDayEngine{

	public:
		GoldDayEngine(std::string windowName, glm::vec2 windowDimentions);
		~GoldDayEngine();

		void Start();

		system::DebugManager& getDebugManager() { return debugManager; }
		system::GraphicsManager& getGraphicsManager() { return graphicsManager; }
	private:
		system::DebugManager debugManager;
		system::GraphicsManager graphicsManager;
	};

}