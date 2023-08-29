#pragma once

#include "../Manager.h"

#include "Window.h"
#include "VulkanInterface/VulkanInterface.h"
#include "../../Systems/Renderer.h"
#include "../../Systems/LightRenderer.h"
#include "Camera.h"
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

		void setCameraAspectRatio(float aspect);
		void drawFrame(float dt);
		void waitIdle() { vkInterface.waitIdle(); }
	private:
		Window window;
		VulkanInterface vkInterface;
		system::Renderer rendererSystem;
		system::LightRenderer lightRendererSystem;
		system::Light lightSystem;
		
	public:
		Camera mainCamera{}; // TODO: make component
	};
}