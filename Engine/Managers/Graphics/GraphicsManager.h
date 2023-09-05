#pragma once

#include "../Manager.h"

#include "EngineIncludes.h"

#include "../../Systems/Renderer.h"
#include "../../Systems/LightRenderer.h"
#include "../../Systems/Light.h"
namespace gde::manager {
	class GraphicsManager : public Manager {
	public:
		GraphicsManager(GoldDayEngine& _engine, std::string windowName, glm::vec2 windowDimentions);
		~GraphicsManager();

		GraphicsManager(const GraphicsManager&) = delete;
		GraphicsManager& operator=(const GraphicsManager&) = delete;

		std::string getType() const override { return typeid(GraphicsManager).name(); };

		Window& getWindow() { return window; }
		VulkanInterface& getVkInterface() { return vkInterface; }

		void drawFrame(float dt);
		void waitIdle() { vkInterface.waitIdle(); }
	private:
		Window window;
		VulkanInterface vkInterface;
	};
}