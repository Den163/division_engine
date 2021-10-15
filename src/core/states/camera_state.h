#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtx/quaternion.hpp>

struct CameraState
{
    glm::vec3 position;
    glm::quat rotation;
    float nearPlane;
    float farPlane;
};