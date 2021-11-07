#pragma once

#include <utils/disable_glm_warnings.h>

#include <configs/renderer_config.h>
#include <configs/window_config.h>
#include <data_structures/versioned_sparse_set.h>
#include <primitives/gui/font.h>
#include <states/platform/glfw_window_state.h>
#include <states/camera_state.h>
#include <states/default_shaders_state.h>
#include <states/shader_pipeline_state.h>
#include <states/input_state.h>
#include <states/raw_input_state.h>
#include <states/renderer_state.h>
#include <states/window_state.h>
#include <states/texture2d_state.h>
#include <states/platform_state.h>
#include <states/resources_state.h>

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