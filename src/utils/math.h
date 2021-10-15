#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Math
{
constexpr glm::vec3 right = { 1,0,0 };
constexpr glm::vec3 up = { 0,1,0 };
constexpr glm::vec3 forward = { 0,0,1 };

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
    auto t = glm::translate(glm::mat4 {1}, translation);
    auto r = glm::toMat4(rotation);
    auto s = glm::scale(glm::mat4 {1}, scale);

    return t * r * s;
}
}

