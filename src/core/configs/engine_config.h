#pragma once

#include "../states/engine_state.h"
#include "renderer_config.h"
#include "shader_config.h"
#include "window_config.h"

#include <functional>
#include <vector>

typedef std::function<void (EngineState& engineState)> EngineCycleFunc;

struct EngineConfig
{
    const RendererConfig renderer;
    const WindowConfig window;
    const std::vector<ShaderConfig> shaders;

    const EngineCycleFunc init = [](auto& state){};
    const EngineCycleFunc preRenderUpdate = [](auto& state){};
    const EngineCycleFunc postRenderUpdate = [](auto& state){};
    const EngineCycleFunc cleanup = [](auto& state){};
};