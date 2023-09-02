#pragma once

#include "../Managers/Graphics/VulkanInterface/Model.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>
#include <bitset>

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

    using id_t = unsigned int;

    using Entity = std::uint32_t;
    const Entity MAX_ENTITIES = 5000;

    const int MAX_COMPONENTS = 16;

    using Signature = std::bitset<MAX_COMPONENTS>;

    class GameObject {
    public:
        using Map = std::unordered_map<id_t, GameObject>; // TODO: change this?


        GameObject(id_t _id) : id{ _id } {}

        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject(GameObject&&) = default;
        GameObject& operator=(GameObject&&) = default;

        id_t getId() { return id; }

        glm::vec3 color{};
        TransformComponent transform{};

        // Optional pointer components
        std::shared_ptr<Model> model{};

        bool hasTransform = false;
        bool hasPointLight = false;

    private:
        

        id_t id;
    };
}