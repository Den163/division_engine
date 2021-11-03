#pragma once

#include <cstdint>
#include "shader_state.h"
#include "shader_pipeline_state.h"
#include "../configs/default_shaders_config.h"
#include "../utils/engine_invariants.h"

struct DefaultShaderState
{
    ShaderState shaders[DefaultShadersConfig::defaultShaders.size()];
    ShaderPipelineState pipeline;

    inline ShaderState& vertex() { return shaders[EngineInvariants::STANDARD_VERTEX_SHADER_INDEX]; }
    inline ShaderState& coloredFragment() { return shaders[EngineInvariants::STANDARD_COLORED_FRAGMENT_SHADER_INDEX]; }
    inline ShaderState& textureFragment() { return shaders[EngineInvariants::STANDARD_TEXTURE_FRAGMENT_SHADER_INDEX]; }
    inline ShaderState& fontFragment() { return shaders[EngineInvariants::STANDARD_FONT_FRAGMENT_SHADER_INDEX]; }
};