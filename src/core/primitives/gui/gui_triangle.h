#pragma once

#include <array>

#include "gui_vertex.h"
#include "../../components/transform.h"

struct GuiTriangle
{
    std::array<GuiVertex, 3> vertices;
};