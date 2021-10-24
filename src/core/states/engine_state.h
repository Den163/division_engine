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
#include "texture2d_state.h"
#include "win32_state.h"

#include <entt/entt.hpp>

struct EngineState
{
    GlfwWindowState glfwWindow;
    Win32State win32;
    WindowState window;

    RendererState renderer;
    CameraState camera;
    UpdateTimeState loopUpdate;
    InputState input;
    RawInputState rawInput;

    std::vector<GlShaderPipelineState> shaderPipelines;
    std::vector<GlShaderState> shaders;
    std::vector<Texture2dState> textures;

    entt::registry guiRegistry;
};