#pragma once

#include "../System.h"

#include "Window.h"
namespace gde::system {
	class GraphicsManager : public System {
	public:
		GraphicsManager(GoldDayEngine& e, std::string windowName, glm::vec2 windowDimentions);
		~GraphicsManager();

		std::string GetType() const override { return typeid(GraphicsManager).name(); };

	private:
		Window window;

		friend class GoldDayEngine;
	};
}