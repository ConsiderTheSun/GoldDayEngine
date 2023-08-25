#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

#include <glm/vec2.hpp>

#include "Engine/GoldDayEngine.h"

static const int WIDTH = 800;
static const int HEIGHT = 600;

void noLinkyErrors() {
    glfwInit(); //

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr); //

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " extensions supported\n";

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
int main() {

    gde::GoldDayEngine gde{"May All Your Days Be Gold", glm::vec2(WIDTH,HEIGHT)};

    gde.Start();

    //gde.debugManager.logger.setChannel(gde::Logger::Verbose, false);

    //noLinkyErrors();

    

    return 0;
}