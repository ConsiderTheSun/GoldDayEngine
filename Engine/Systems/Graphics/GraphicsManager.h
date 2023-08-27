#pragma once

#include "../System.h"

#include "Window.h"
#include "VulkanInterface/VulkanInterface.h"
#include "Renderer.h"
#include "Camera.h"
namespace gde::system {
	class GraphicsManager : public System {
	public:
		GraphicsManager(GoldDayEngine& _engine, std::string windowName, glm::vec2 windowDimentions);
		~GraphicsManager();

		GraphicsManager(const GraphicsManager&) = delete;
		GraphicsManager& operator=(const GraphicsManager&) = delete;

		std::string getType() const override { return typeid(GraphicsManager).name(); };

		Window& getWindow() { return window; }
		VulkanInterface& getVkInterface() { return vkInterface; }

		void updateCamera();
		void drawFrame();
		void waitIdle() { vkInterface.waitIdle(); }
	private:
		Window window;
		VulkanInterface vkInterface{engine,window };
		Renderer renderer;

	public:
		Camera mainCamera{}; // TODO: make component
	};
}