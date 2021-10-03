#pragma once

#include "components/renderer_config.h"
#include "components/renderer_state.h"
#include "components/window_config.h"
#include "components/window_state.h"
#include "components/glfw_window_data.h"
#include "components/gl_shader_state.h"

#include "entt/entt.hpp"

struct EngineState
{
    GlfwWindowData glfwWindowData;
    WindowConfig windowConfig;
    WindowState windowState;

    RendererConfig rendererConfig;
    RendererState rendererState;
    UpdateTimestamp loopUpdateTimestamp;
    GlShaderState shaderState;

    entt::registry ecsRegistry;
};