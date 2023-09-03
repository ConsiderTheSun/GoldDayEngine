#include "EngineIncludes.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

#include <glm/vec2.hpp>

#include "EngineInterface/EngineInterface.h"
#include "Applications/BasicTesting/BasicTestingApp.h"


int main() {
    const int DEFAULT_WIDTH = 800;
    const int DEFAULT_HEIGHT = 600;

	std::string d = "";
	std::cout << "Choose App:\n";
	std::cout << "T: Madz' Basic Testing \n";
	std::cout << "Enter: ";

	//std::cin >> d;
    d = "t";
	std::cout << "\n";

    gde::EngineInterface iEngine{ "May All Your Days Be Gold", glm::vec2(DEFAULT_WIDTH,DEFAULT_HEIGHT) };

    if (d == "T" || d == "t") {
        BasicTestingApp app(iEngine);
        app.setup();

        iEngine.setApp(&app); // TODO: remove when scripting is added
        
        iEngine.start();
    }

    return 0;
}