#pragma once

#include <cstdint>
#include <vector>

#include "../primitives/gui/gui_vertex.h"
#include "../configs/render_mode.h"

struct GuiMesh
{
    static constexpr size_t SHADER_INDEX = 0;

    RenderMode renderShape;
    std::vector<GuiVertex> vertices;
};