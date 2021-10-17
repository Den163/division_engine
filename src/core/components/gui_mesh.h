#pragma once

#include <vector>
#include "../primitives/gui/gui_vertex.h"
#include "glad/gl.h"
#include "render_mode.h"

struct GuiMesh
{
    RenderMode renderShape;
    std::vector<GuiVertex> vertices;
};