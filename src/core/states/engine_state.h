#pragma once

#include "../configs/renderer_config.h"
#include "../configs/window_config.h"
#include "glfw_window_state.h"

#include "camera_state.h"
#include "default_shaders_state.h"
#include "shader_pipeline_state.h"
#include "input_state.h"
#include "raw_input_state.h"
#include "renderer_state.h"
#include "window_state.h"
#include "texture2d_state.h"
#include "win32_state.h"
#include "../data_structures/versioned_sparse_set.h"
#include "../primitives/gui/font.h"

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
    DefaultShaderState defaultShader;

    VersionedSparseSet<Font> fonts { 10 };

    entt::registry guiRegistry;
};