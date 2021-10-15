#pragma once

#include <array>

#include "gui_vertex.h"
#include "transform.h"

struct Triangle
{
    Transform transform;
    std::array<GuiVertex, 3> vertices;
};