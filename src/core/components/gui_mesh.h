#pragma once

#include <vector>
#include "gui_vertex.h"
#include "glad/gl.h"
#include "render_shape.h"

struct GuiMesh
{
    RenderShape renderShape;
    std::vector<GuiVertex> vertices;
};