#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <string>

namespace gde {
	class GoldDayEngine;

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

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		glm::vec2 getDimentions() { return dimentions; }
		VkExtent2D getExtent() { return { static_cast<uint32_t>(dimentions.x), static_cast<uint32_t>(dimentions.y) }; }
		
		bool wasWindowResized() { return framebufferResized; }
		void resetWindowResizedFlag() { framebufferResized = false; }
	
	private:
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

		GLFWwindow* window;

		GoldDayEngine& engine;

		glm::vec2 dimentions;
		bool framebufferResized = false;

	};
}