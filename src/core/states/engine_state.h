#pragma once

#include "../configs/renderer_config.h"
#include "../configs/window_config.h"
#include "glfw_window_state.h"

#include "camera_state.h"
#include "gl_shader_state.h"
#include "input_state.h"
#include "raw_input_state.h"
#include "renderer_state.h"
#include "window_state.h"

#include <entt/entt.hpp>

struct EngineState
{
    GlfwWindowState glfwWindowData;
    WindowState windowState;

    RendererState rendererState;
    CameraState cameraState;
    UpdateTimeState loopUpdateTimestamp;
    GlShaderState shaderState;
    InputState inputState;
    RawInputState rawInputState;

    entt::registry ecsRegistry;
};