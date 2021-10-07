#pragma once

#include "../configs/renderer_config.h"
#include "../configs/window_config.h"
#include "../configs/glfw_window_config.h"

#include "camera_state.h"
#include "gl_shader_state.h"
#include "input_state.h"
#include "raw_input_state.h"
#include "renderer_state.h"
#include "window_state.h"

#include <entt/entt.hpp>

struct EngineState
{
    GlfwWindowConfig glfwWindowData;
    WindowConfig windowConfig;
    WindowState windowState;

    RendererConfig rendererConfig;
    RendererState rendererState;
    CameraState cameraState;
    UpdateTimeState loopUpdateTimestamp;
    GlShaderState shaderState;
    InputState inputState;
    RawInputState rawInputState;

    entt::registry ecsRegistry;
};