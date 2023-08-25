#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <string>
namespace gde { class GoldDayEngine; }

namespace gde {
	/*
	 * @brief Encapsulation of GLFW
	 */
	class Window {
	public:
		Window(GoldDayEngine& e, std::string windowName, glm::vec2 windowDimentions);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		bool shouldClose() const { return glfwWindowShouldClose(window); }
		void pollEvents() { glfwPollEvents(); }

	private:
		GLFWwindow* window;

		GoldDayEngine& engine;
	};
}