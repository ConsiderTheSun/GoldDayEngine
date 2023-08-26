#include "Window.h"



#include "../GoldDayEngine.h"
namespace gde {
	Window::Window(GoldDayEngine& e, std::string windowName, glm::vec2 windowDimentions) : engine(e), dimentions(windowDimentions) {
		engine.getDebugManager().getLogger().Log(Logger::Verbose, "Window Instantiated");

		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		

		window = glfwCreateWindow(windowDimentions.x, windowDimentions.y, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

		glfwSetWindowAspectRatio(window, dimentions.x, dimentions.y);
	}
	Window::~Window() {
		engine.getDebugManager().getLogger().Log(Logger::Verbose, "Window Freed");

		glfwDestroyWindow(window);
		glfwTerminate();
	}


	void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			engine.getDebugManager().getLogger().Log(Logger::Error, "Window Surface Not Created");
		}
	}

	void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto gdeWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		gdeWindow->framebufferResized = true;
		gdeWindow->dimentions.x = width;
		gdeWindow->dimentions.y = height;
	}
}

