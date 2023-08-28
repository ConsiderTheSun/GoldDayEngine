#pragma once

#include "../Systems/Graphics/VulkanInterface/Model.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>

namespace gde {

    struct TransformComponent {
        glm::vec3 translation{};

        // only allowing uniform scale allows for passing the cheaper mat4() to shaders instead of normalMatrix()
        // https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
        float scale{ 1.0f }; 

        glm::vec3 rotation{};

        
        glm::mat4 mat4();
        glm::mat3 normalMatrix();
    };

    struct PointLightComponent {
        float lightIntensity = 1.0f;
    };

    class GameObject {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, GameObject>; // TODO: change this?

        static GameObject createGameObject() { // TODO: move to GOF
            static id_t currentId = 0;
            return GameObject{ currentId++ };
        }

        static GameObject makePointLight(
            float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f)); // TODO: move to GOF

        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject(GameObject&&) = default;
        GameObject& operator=(GameObject&&) = default;

        id_t getId() { return id; }

        glm::vec3 color{};
        TransformComponent transform{};

        // Optional pointer components
        std::shared_ptr<Model> model{};
        std::unique_ptr<PointLightComponent> pointLight = nullptr;

    private:
        GameObject(id_t _id) : id{ _id } {}

        id_t id;
    };
}