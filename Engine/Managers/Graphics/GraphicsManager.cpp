#include "EngineIncludes.h"

#include "GraphicsManager.h"
#include "../GoldDayEngine.h"

namespace gde::manager {


	GraphicsManager::GraphicsManager(GoldDayEngine& _engine, std::string windowName, glm::vec2 windowDimentions) :
		Manager(_engine),
		window(_engine, windowName, windowDimentions),
		vkInterface{ engine,window }
	{
		engine.getDebugManager().getLogger().log(Logger::Verbose, "GraphicsManager Instantiated");
	}

	GraphicsManager::~GraphicsManager() {
		engine.getDebugManager().getLogger().log(Logger::Verbose, "GraphicsManager Freed");
	}

	void GraphicsManager::drawFrame(float dt) {
		auto commandBuffer = vkInterface.beginFrame();
		if (commandBuffer) {

			// sets ubo data
			GlobalUbo ubo{};
			engine.getMOM().getSystem<system::CameraUpdate>()->setCameraUBOData(ubo);
			engine.getMOM().getSystem<system::Light>()->updateLightData(ubo);
			

			// writes to gpu
			vkInterface.setUboData(ubo);

			vkInterface.beginSwapChainRenderPass(commandBuffer);

			engine.getMOM().getSystem<system::Renderer>()->renderGameObjects();
			if (engine.getDebugManager().isLightRenderingEnabled()) {
				engine.getMOM().getSystem<system::LightRenderer>()->renderLights();
			}

			vkInterface.endSwapChainRenderPass(commandBuffer);
			vkInterface.endFrame();
		}
	}
}