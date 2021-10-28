#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtx/quaternion.hpp>

struct CameraState
{
    glm::vec3 position = glm::vec3 {0};
    glm::quat rotation = glm::identity<glm::quat>();
};