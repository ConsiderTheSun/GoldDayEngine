#include "EngineIncludes.h"

#include "GraphicsManager.h"
#include "../GoldDayEngine.h"

namespace gde::manager {


	GraphicsManager::GraphicsManager(GoldDayEngine& _engine, std::string windowName, glm::vec2 windowDimentions) :
		Manager(_engine),
		window(_engine, windowName, windowDimentions),
		vkInterface{ engine,window },
		rendererSystem{ _engine },
		lightRendererSystem{ _engine },
		lightSystem{ _engine }
	{
		engine.getDebugManager().getLogger().log(Logger::Verbose, "GraphicsManager Instantiated");
	}

	GraphicsManager::~GraphicsManager() {
		engine.getDebugManager().getLogger().log(Logger::Verbose, "GraphicsManager Freed");
	}

	void GraphicsManager::setCameraAspectRatio(float aspect) {
		mainCamera.setPerspectiveProjection(aspect, 0.1f, 100.f);
	}

	void GraphicsManager::drawFrame(float dt) {
		auto commandBuffer = vkInterface.beginFrame();
		if (commandBuffer) {

			lightSystem.updateLightData(mainCamera);

			vkInterface.beginSwapChainRenderPass(commandBuffer);

			rendererSystem.renderGameObjects();
			if (engine.getDebugManager().isLightRenderingEnabled()) {
				lightRendererSystem.renderLights();
			}

			vkInterface.endSwapChainRenderPass(commandBuffer);
			vkInterface.endFrame();
		}
	}
}