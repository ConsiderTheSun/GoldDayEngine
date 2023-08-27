#pragma once

#include "Systems/Graphics/VulkanInterface/Model.h"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

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

    class GameObject {
    public:
        using id_t = unsigned int;

        static GameObject createGameObject() {
            static id_t currentId = 0;
            return GameObject{ currentId++ };
        }

        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject(GameObject&&) = default;
        GameObject& operator=(GameObject&&) = default;

        id_t getId() { return id; }

        std::shared_ptr<Model> model{};
        glm::vec3 color{};
        TransformComponent transform{};

    private:
        GameObject(id_t _id) : id{ _id } {}

        id_t id;
    };
}