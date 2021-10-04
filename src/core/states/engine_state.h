#pragma once

#include "../configs/renderer_config.h"
#include "renderer_state.h"
#include "../configs/window_config.h"
#include "window_state.h"
#include "../configs/glfw_window_config.h"
#include "gl_shader_state.h"

#include "entt/entt.hpp"

struct EngineState
{
    GlfwWindowConfig glfwWindowData;
    WindowConfig windowConfig;
    WindowState windowState;

    RendererConfig rendererConfig;
    RendererState rendererState;
    UpdateTimeState loopUpdateTimestamp;
    GlShaderState shaderState;

    entt::registry ecsRegistry;
};