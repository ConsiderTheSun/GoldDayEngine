#include "Window.h"



#include "../GoldDayEngine.h"
namespace gde {
	Window::Window(GoldDayEngine& e, std::string windowName, glm::vec2 windowDimentions) : engine(e) {
		engine.getDebugManager().getLogger().Log(Logger::Verbose, "Window Instantiated");

		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(windowDimentions.x, windowDimentions.y, windowName.c_str(), nullptr, nullptr);

	}
	Window::~Window() {
		engine.getDebugManager().getLogger().Log(Logger::Verbose, "Window Freed");

		glfwDestroyWindow(window);
		glfwTerminate();
	}
}