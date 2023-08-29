#pragma once

#include "glm/glm/vec3.hpp"

namespace gde::component {
    struct Transform {
        glm::vec3 translation{};

        // only allowing uniform scale allows for passing the cheaper mat4() to shaders instead of normalMatrix()
        // https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
        float scale{ 1.0f };

        glm::vec3 rotation{};
    };
}