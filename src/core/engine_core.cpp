#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

#include "engine_core.h"

#include <iostream>

#include "states/engine_state.h"
#include "systems/gl_shader_program_system.h"
#include "systems/glfw_window_system.h"
#include "systems/render_tick_system.h"
#include "systems/gl_render_system.h"
#include "systems/check_gl_mesh_created_system.h"
#include "systems/check_gl_mesh_destroyed_system.h"
#include "systems/glfw_vsync_system.h"
#include "systems/loop_tick_system.h"
#include "systems/win32_register_input_system.h"
#include "systems/register_input_system.h"

#include "../hook/lifecycle.h"

static void init(EngineState& state);
static void eventLoop(EngineState& state);
static void cleanup(EngineState& state);

void EngineCore::run(WindowConfig& windowConfig, RendererConfig& rendererConfig)
{
    EngineState engineState {};
    engineState.windowConfig = windowConfig;
    engineState.rendererConfig = rendererConfig;

    init(engineState);
    eventLoop(engineState);
    cleanup(engineState);
}

static void init(EngineState& state)
{
    Win32RegisterInputSystem::init(state.rawInputState);
    RegisterInputSystem::init(state.inputState);
    LoopTickSystem::init(state.loopUpdateTimestamp);
    RenderTickSystem::init(state.rendererState);
    GlfWindowSystem::init(state.windowState, state.glfwWindowData, state.windowConfig);
    GlShaderProgramSystem::init(state.shaderState);
    GlRenderSystem::init(state.shaderState, state.windowConfig);

    Lifecycle::init(state);

#ifdef PRINT_OPENGL_INFO
    DebugUtils::printRendererInfo();
#endif
}

static void eventLoop(EngineState& state)
{
    const auto& rendererState = state.rendererState;
    const auto& windowState = state.windowState;

    do
    {
        LoopTickSystem::update(state.loopUpdateTimestamp);
        RenderTickSystem::update(state.rendererState, state.rendererConfig, state.loopUpdateTimestamp);
        Win32RegisterInputSystem::update(state.rawInputState);
        RegisterInputSystem::eventLoopUpdate(state.inputState, state.rawInputState);

        if (rendererState.shouldUpdate)
        {
            Lifecycle::preRenderUpdate(state);

            CheckGlMeshCreatedSystem::update(state.ecsRegistry, state.shaderState);
            CheckGlMeshDestroyedSystem::update(state.ecsRegistry, state.shaderState);

            GlRenderSystem::update(state.ecsRegistry, state.shaderState, state.rendererConfig);
            GlfwVsyncSystem::update(state.glfwWindowData);

            Lifecycle::postRenderUpdate(state);
            RegisterInputSystem::postRenderUpdate(state.inputState);
        }

        GlfWindowSystem::update(state.windowState, state.glfwWindowData);
    }
    while (!windowState.shouldClose);
}

static void cleanup(EngineState& state)
{
    Lifecycle::cleanup(state);
    GlShaderProgramSystem::cleanup(state.shaderState);
    GlfWindowSystem::cleanup(state.glfwWindowData);
}