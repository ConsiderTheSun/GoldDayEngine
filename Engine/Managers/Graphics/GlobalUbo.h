#pragma once

#include <glm/glm.hpp>


#define MAX_LIGHTS 10

namespace gde {
	struct PointLight {
		glm::vec4 position{};  // ignore w
		glm::vec4 color{};     // w is intensity
	};

	struct GlobalUbo {
		glm::mat4 projectionView{ 1.f };
		glm::mat4 view{ 1.f };
		glm::mat4 inverseView{ 1.f };
		glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };  // w is intensity
		PointLight pointLights[MAX_LIGHTS];
		int numLights;
	};
}