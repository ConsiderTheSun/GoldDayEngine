#include "EngineIncludes.h"
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

int main() {

    gde::GoldDayEngine gde{"May All Your Days Be Gold", glm::vec2(WIDTH,HEIGHT)};

    gde.start();

    //gde.debugManager.logger.setChannel(gde::Logger::Verbose, false);

    

    return 0;
}