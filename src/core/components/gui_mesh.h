#pragma once

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <vector>

#include "../primitives/gui/gui_vertex.h"
#include "../configs/render_mode.h"
#include "../utils/engine_invariants.h"

struct GuiMesh
{
    uint32_t verticesCount;
    uint32_t primitivesCount;
};