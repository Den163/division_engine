#pragma once

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <vector>

#include "../primitives/gui/gui_vertex.h"
#include "../configs/render_mode.h"
#include "../../utils/engine_invariants.h"

struct GuiMesh
{
    RenderMode renderMode;
    uint32_t shaderPipelineIndex = EngineInvariants::STANDARD_SHADER_PIPELINE_INDEX;
    uint32_t vertexShaderIndex = EngineInvariants::STANDARD_VERTEX_SHADER_INDEX;
    uint32_t fragmentShaderIndex = EngineInvariants::STANDARD_COLORED_FRAGMENT_SHADER_INDEX;

    std::vector<GuiVertex> vertices;
};