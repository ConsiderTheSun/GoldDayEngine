#include "Window.h"



#include "../GoldDayEngine.h"
namespace gde {
	bool Window::scrollChange = false;
	float Window::deltaScroll = 0.0f;

	Window::Window(GoldDayEngine& _engine, std::string windowName, glm::vec2 windowDimentions) : engine{ _engine }, dimentions(windowDimentions) {
		engine.getDebugManager().getLogger().log(Logger::Verbose, "Window Instantiated");
		
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		

		window = glfwCreateWindow(windowDimentions.x, windowDimentions.y, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

		glfwSetWindowAspectRatio(window, dimentions.x, dimentions.y);

		glfwSetScrollCallback(window, scrollCallback);

	}
	Window::~Window() {
		engine.getDebugManager().getLogger().log(Logger::Verbose, "Window Freed");

		glfwDestroyWindow(window);
		glfwTerminate();
	}


	void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			engine.getDebugManager().getLogger().log(Logger::Error, "Window Surface Not Created");
		}
	}

	

	bool Window::getButton(int buttonID) const{
		if (buttonID <= GLFW_MOUSE_BUTTON_LAST) {
			return glfwGetMouseButton(window, buttonID);
		}
		else {
			if (glfwGetKey(window, buttonID) == GLFW_PRESS) {
				int i = 0;
			}
			return glfwGetKey(window, buttonID) == GLFW_PRESS;
		}
	}

	glm::vec2 Window::getCursorPos() const{
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return glm::vec2(xPos, yPos);
	}

	void Window::setMouseMode(MouseMode mode) {
		glfwSetInputMode(window, GLFW_CURSOR, mode);
	}

	void Window::resetScrollData() {
		scrollChange = false;
		deltaScroll = 0.0f;
	}

	void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto gdeWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		gdeWindow->framebufferResized = true;
		gdeWindow->dimentions.x = width;
		gdeWindow->dimentions.y = height;
	}

	void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		scrollChange = true;
		deltaScroll = yoffset;
	}

}

