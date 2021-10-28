#pragma once

#include <glm/vec3.hpp>

struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    /*
     * Returns transform with zero position, zero rotation and identity scale
    */
    static constexpr inline Transform makeDefault() { return Transform { {}, {}, glm::vec3{1.f} }; }

    Transform withPosition(const glm::vec3& position) const
    {
        return Transform { position, this->rotation, this->scale };
    }

    Transform withRotation(const glm::vec3& rotation) const
    {
        return Transform { this->position, rotation, this->scale };
    }

    Transform withScale(const glm::vec3& scale) const
    {
        return Transform { this->position, this->rotation, scale };
    }
};