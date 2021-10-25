#pragma once

#include <cstdint>
#include "../configs/default_shaders_config.h"
#include "shader_state.h"
#include "shader_pipeline_state.h"

struct DefaultShaderState
{
    ShaderState shaders[DefaultShadersConfig::defaultShaders.size()];
    ShaderPipelineState pipeline;
};