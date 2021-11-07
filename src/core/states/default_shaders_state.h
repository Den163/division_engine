#pragma once

#include <cstdint>

#include <configs/default_shaders_config.h>
#include <states/shader_state.h>
#include <states/shader_pipeline_state.h>
#include <utils/engine_invariants.h>

struct DefaultShaderState
{
    ShaderState shaders[DefaultShadersConfig::defaultShaders.size()];
    ShaderPipelineState pipeline;

    inline const ShaderState& vertex() const
    { return shaders[EngineInvariants::STANDARD_VERTEX_SHADER_INDEX]; }
    inline const ShaderState& coloredFragment() const
    { return shaders[EngineInvariants::STANDARD_COLORED_FRAGMENT_SHADER_INDEX]; }
    inline const ShaderState& textureFragment() const
    { return shaders[EngineInvariants::STANDARD_TEXTURE_FRAGMENT_SHADER_INDEX]; }
    inline const ShaderState& fontFragment() const
    { return shaders[EngineInvariants::STANDARD_FONT_FRAGMENT_SHADER_INDEX]; }
};