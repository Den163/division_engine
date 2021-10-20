#pragma once

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <vector>

#include "../primitives/gui/gui_vertex.h"
#include "../configs/render_mode.h"

struct GuiMesh
{
    static constexpr size_t SHADER_INDEX = 0;

    RenderMode renderMode;
    std::vector<GuiVertex> vertices;
    glm::mat4 modelViewProjection;
};