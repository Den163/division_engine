#pragma once

#include "../configs/renderer_config.h"
#include "../configs/window_config.h"
#include "glfw_window_state.h"

#include "camera_state.h"
#include "gl_shader_pipeline_state.h"
#include "input_state.h"
#include "raw_input_state.h"
#include "renderer_state.h"
#include "window_state.h"
#include "win32_state.h"

#include <entt/entt.hpp>

struct EngineState
{
    GlfwWindowState glfwWindowState;
    Win32State win32State;
    WindowState windowState;

    RendererState rendererState;
    CameraState cameraState;
    UpdateTimeState loopUpdateTimestamp;
    InputState inputState;
    RawInputState rawInputState;

    std::vector<GlShaderPipelineState> shaderPipelineStates;
    std::vector<GlShaderState> shaderStates;

    entt::registry guiRegistry;
};