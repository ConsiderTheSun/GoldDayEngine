#pragma once

#include "../../EngineIncludes.h"

namespace gde::component {
    struct Transform {
        glm::vec3 translation{};
        glm::vec3 rotation{};

        // only allowing uniform scale allows for passing the cheaper mat4() to shaders instead of normalMatrix()
        // https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
        float scale{ 1.0f };
    };
}