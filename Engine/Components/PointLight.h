#pragma once

namespace gde::component {
    struct PointLight {
        float lightIntensity = 1.0f;
        glm::vec3 color{};
    };
}