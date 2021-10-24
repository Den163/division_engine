#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct GuiVertex
{
    glm::vec4 color;
    glm::vec3 position;
    glm::vec2 uv;
};