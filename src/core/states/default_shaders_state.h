#pragma once

#include <cstdint>
#include "shader_state.h"
#include "shader_pipeline_state.h"
#include "../configs/default_shaders_config.h"

struct DefaultShaderState
{
    ShaderState shaders[DefaultShadersConfig::defaultShaders.size()];
    ShaderPipelineState pipeline;
};