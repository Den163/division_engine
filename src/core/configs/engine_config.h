#pragma once

#include "../states/engine_state.h"
#include "lifecycle_config.h"
#include "renderer_config.h"
#include "shader_config.h"
#include "shader_pipeline_config.h"
#include "texture2d_config.h"
#include "window_config.h"

#include <functional>
#include <vector>

struct EngineConfig
{
    RendererConfig renderer;
    WindowConfig window;
    ShaderPipelineConfig shaderPipeline;

    std::vector<ShaderConfig> shaders;
    std::vector<Texture2dConfig> textures;
};