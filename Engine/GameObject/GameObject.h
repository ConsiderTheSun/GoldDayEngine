#pragma once

#include "../Managers/Graphics/VulkanInterface/Model.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>
#include <bitset>

namespace gde {
    using ComponentType = std::uint8_t;
    using GOID = std::uint32_t;
    const std::uint32_t MAX_ENTITIES = 5000;

    const int MAX_COMPONENTS = 16;

    using Signature = std::bitset<MAX_COMPONENTS>;

    class GameObject {
    public:


        GameObject(GOID _id) : id{ _id } {}

        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject(GameObject&&) = default;
        GameObject& operator=(GameObject&&) = default;

        GOID getId() const { return id; }

        const Signature& getSignature() { return signature; }
        void addToSignature(ComponentType type) { signature.set(type, true); }

    private:
        

        GOID id;


        Signature signature;
    };
}