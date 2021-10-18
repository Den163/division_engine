#pragma once

#include "renderer_config.h"
#include "shader_config.h"
#include "window_config.h"

#include <vector>

struct EngineConfig
{
    const RendererConfig renderer;
    const WindowConfig window;
    const std::vector<ShaderConfig> shaders;
};