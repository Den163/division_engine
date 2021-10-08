#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

template<typename T>
inline constexpr bool hasFlag(T mask, T flag) { return (mask & flag) == flag; }

template<typename T>
inline constexpr T selectScalar(bool predicate, T ifTrue, T ifFalse)
{
    return predicate * ifTrue + (1 - predicate) * ifFalse;
}

inline glm::mat4 transformMatrix(
    const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale = glm::vec3 {1})
{
    auto scaleMat = glm::scale(glm::mat4 {1}, scale);
    auto rotMat = glm::mat4_cast(rotation);
    return glm::translate(scaleMat * rotMat , translation);
}