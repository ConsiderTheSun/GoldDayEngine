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
		enum MouseMode
		{
			NORMAL = GLFW_CURSOR_NORMAL,
			LOCKED = GLFW_CURSOR_DISABLED,
			HIDDEN = GLFW_CURSOR_HIDDEN
		};
		Window(GoldDayEngine& _engine, std::string windowName, glm::vec2 windowDimentions);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		bool shouldClose() const { return glfwWindowShouldClose(window); }
		void setClose() { glfwSetWindowShouldClose(window, true); }

		void pollEvents() { glfwPollEvents(); }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		glm::vec2 getDimentions() { return dimentions; }
		VkExtent2D getExtent() { return { static_cast<uint32_t>(dimentions.x), static_cast<uint32_t>(dimentions.y) }; }
		
		bool wasWindowResized() { return framebufferResized; }
		void resetWindowResizedFlag() { framebufferResized = false; }

		// window dependent inputs
		bool getButton(int buttonID) const;
		glm::vec2 getCursorPos() const;
		void setMouseMode(MouseMode mode);

		std::pair<bool, int> getScrollData() const { return { scrollChange,deltaScroll }; }
		static void resetScrollData();

	private:
		

		GLFWwindow* window;

		GoldDayEngine& engine;

		glm::vec2 dimentions;
		bool framebufferResized = false;

		static bool scrollChange;
		static float deltaScroll;

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		
		static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	};
}