#pragma once

#include "components/renderer_config.h"
#include "components/renderer_state.h"
#include "components/window_config.h"
#include "components/window_state.h"
#include "components/glfw_window_data.h"
#include "components/gl_shader_state.h"

struct EngineState
{
    GlfwWindowData glfwWindowData;
    WindowConfig windowConfig;
    WindowState windowState;

    RendererConfig rendererConfig;
    RendererState rendererState;
    UpdateTimestamp loopUpdateTimestamp;
    GlShaderState shaderState;
};