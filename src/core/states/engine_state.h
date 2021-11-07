#pragma once

#include "../utils/disable_glm_warnings.h"

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
#include "platform_state.h"
#include "../data_structures/versioned_sparse_set.h"
#include "../primitives/gui/font.h"
#include "resources_state.h"

#include <entt/entt.hpp>

struct EngineState
{
    PlatformState platform;
    WindowState window;

    RendererState renderer;
    CameraState camera;
    UpdateDeltaTimeState loopUpdate;
    InputState input;
    RawInputState rawInput;
    DefaultShaderState defaultShader;
    ResourcesState resources;

    entt::registry guiRegistry;
};