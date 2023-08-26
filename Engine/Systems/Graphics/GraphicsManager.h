#pragma once

#include "../System.h"

#include "Window.h"
#include "VulkanInterface/VulkanInterface.h"
namespace gde::system {
	class GraphicsManager : public System {
	public:
		GraphicsManager(GoldDayEngine& e, std::string windowName, glm::vec2 windowDimentions);
		~GraphicsManager();

		GraphicsManager(const GraphicsManager&) = delete;
		GraphicsManager& operator=(const GraphicsManager&) = delete;

		std::string GetType() const override { return typeid(GraphicsManager).name(); };

		Window& getWindow() { return window; }

		void drawFrame();
		void waitIdle() { vkInterface.waitIdle(); }
	private:
		Window window;
		VulkanInterface vkInterface{engine,window };

		friend class GoldDayEngine;
	};
}