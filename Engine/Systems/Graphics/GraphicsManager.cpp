#include "GraphicsManager.h"
#include "../GoldDayEngine.h"

namespace gde::system {
	

	GraphicsManager::GraphicsManager(GoldDayEngine& _engine, std::string windowName, glm::vec2 windowDimentions) :
			System(_engine), 
			window(_engine, windowName, windowDimentions), 
			vkInterface{ engine,window },
			renderer{ _engine,vkInterface.getDevice(), vkInterface.getSwapChainRenderPass(),vkInterface.getGlobalDescriptorSetLayout() }{
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

			updateUbo();

			vkInterface.beginSwapChainRenderPass(commandBuffer);

			renderer.renderGameObjects();
			if (engine.getDebugManager().isLightRenderingEnabled()) {
				renderer.renderLights();
			}

			vkInterface.endSwapChainRenderPass(commandBuffer);
			vkInterface.endFrame();
		}
	}

	void GraphicsManager::updateUbo() {
		VulkanInterface::GlobalUbo ubo{};

		// update camera stuff
		ubo.projectionView = mainCamera.getProjection() * mainCamera.getView();
		ubo.view = mainCamera.getView();
		ubo.inverseView = mainCamera.getInverseView();

		// update light stuff
		int lightIndex = 0;
		for (auto& kv : engine.gameObjects) {
			auto& obj = kv.second;
			if (obj.pointLight == nullptr) continue;

			assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

			// copy light to ubo
			ubo.pointLights[lightIndex].position = glm::vec4(obj.transform.translation, 1.f);
			ubo.pointLights[lightIndex].color = glm::vec4(obj.color, obj.pointLight->lightIntensity);

			lightIndex += 1;
		}
		ubo.numLights = lightIndex;

		// writes to gpu
		vkInterface.setUboData(ubo);
	}

}