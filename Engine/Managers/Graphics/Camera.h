#pragma once

#include "../EngineIncludes.h"
namespace gde {

    class Camera {
    public:
        void setOrthographicProjection(
            float left, float right, float top, float bottom, float cameraNear, float cameraFar);

        void setPerspectiveProjection(float aspect, float cameraNear, float cameraFar);
        
        void setViewDirection(
            glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{ 0.f, -1.f, 0.f });
        void setViewTarget(
            glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.f, -1.f, 0.f });
        void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

        const glm::mat4& getProjection() const { return projectionMatrix; }
        const glm::mat4& getView() const { return viewMatrix; }
        const glm::mat4& getInverseView() const { return inverseViewMatrix; }
        float fovy = glm::radians(50.0f);
    private:
        glm::mat4 projectionMatrix{ 1.f };
        glm::mat4 viewMatrix{ 1.f };
        glm::mat4 inverseViewMatrix{ 1.f };
    };
}