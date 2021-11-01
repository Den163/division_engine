#pragma once

#include <glm/vec2.hpp>

// Contains data defining how to position child gui element relative to parent
// (interpolates between corners - {0;0} - left bottom corner, {1;1} - right top corner
struct GuiAnchor
{
    glm::vec2 parent;
    glm::vec2 child;
};