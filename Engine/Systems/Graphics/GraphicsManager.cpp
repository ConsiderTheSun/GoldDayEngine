#include "GraphicsManager.h"
#include "../GoldDayEngine.h"

namespace gde::system {
	GraphicsManager::GraphicsManager(GoldDayEngine& _engine, std::string windowName, glm::vec2 windowDimentions) :
			System(_engine), 
			window(_engine, windowName, windowDimentions), 
			renderer{ _engine,vkInterface.getDevice(), vkInterface.getSwapChainRenderPass() } {
		engine.getDebugManager().getLogger().log(Logger::Verbose, "GraphicsManager Instantiated");
		//mainCamera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
		mainCamera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));
	}

	GraphicsManager::~GraphicsManager() {
		engine.getDebugManager().getLogger().log(Logger::Verbose, "GraphicsManager Freed");
	}

	void GraphicsManager::updateCamera() {
		float aspect = vkInterface.getAspectRatio();
		mainCamera.setPerspectiveProjection(aspect, 0.1f, 10.f);
	}
	void GraphicsManager::drawFrame() {

		if (auto commandBuffer = vkInterface.beginFrame()) {
			vkInterface.beginSwapChainRenderPass(commandBuffer);

			renderer.renderGameObjects(commandBuffer, engine.gameObjects, mainCamera);
				
			vkInterface.endSwapChainRenderPass(commandBuffer);
			vkInterface.endFrame();
		}
	}

}