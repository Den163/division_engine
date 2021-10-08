#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

#include "engine_core.h"

#include <iostream>

#include "states/engine_state.h"
#include "systems/rendering/gl_shader_program_system.h"
#include "systems/glfw_window_system.h"
#include "systems/rendering/render_tick_system.h"
#include "systems/rendering/gl_render_system.h"
#include "systems/rendering/check_gl_mesh_created_system.h"
#include "systems/rendering/check_gl_mesh_destroyed_system.h"
#include "systems/glfw_vsync_system.h"
#include "systems/loop_tick_system.h"
#include "systems/input/win32_register_input_system.h"
#include "systems/input/register_input_system.h"

#include "../hook/lifecycle.h"

static void init(EngineState& state, const WindowConfig& windowConfig, const RendererConfig& rendererConfig);
static void eventLoop(EngineState& state);
static void renderLoop(EngineState& state);
static void cleanup(EngineState& state);

void EngineCore::run(WindowConfig& windowConfig, RendererConfig& rendererConfig)
{
    EngineState engineState {};

    init(engineState, windowConfig, rendererConfig);
    eventLoop(engineState);
    cleanup(engineState);
}

static void init(EngineState& state, const WindowConfig& windowConfig, const RendererConfig& rendererConfig)
{
    Win32RegisterInputSystem::init(state.rawInputState);
    RegisterInputSystem::init(state.inputState);
    LoopTickSystem::init(state.loopUpdateTimestamp);
    RenderTickSystem::init(state.rendererState, rendererConfig);
    GlfWindowSystem::init(state.windowState, state.glfwWindowData, windowConfig);
    GlShaderProgramSystem::init(state.shaderState);
    GlRenderSystem::init(state.shaderState, state.cameraState, state.windowState);

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
        RenderTickSystem::update(state.rendererState, state.loopUpdateTimestamp);
        Win32RegisterInputSystem::update(state.rawInputState);
        RegisterInputSystem::eventLoopUpdate(state.inputState, state.rawInputState);

        if (rendererState.shouldUpdate)
        {
            renderLoop(state);
        }

        GlfWindowSystem::update(state.windowState, state.glfwWindowData);
    }
    while (!windowState.shouldClose);
}

void renderLoop(EngineState& state)
{
    Lifecycle::preRenderUpdate(state);

    CheckGlMeshCreatedSystem::update(state.ecsRegistry, state.shaderState);
    CheckGlMeshDestroyedSystem::update(state.ecsRegistry, state.shaderState);

    GlRenderSystem::update(
        state.ecsRegistry,
        state.shaderState,
        state.rendererState,
        state.cameraState,
        state.windowState);
    GlfwVsyncSystem::update(state.glfwWindowData);

    Lifecycle::postRenderUpdate(state);
    RegisterInputSystem::postRenderUpdate(state.inputState);
}

static void cleanup(EngineState& state)
{
    Lifecycle::cleanup(state);
    GlShaderProgramSystem::cleanup(state.shaderState);
    GlfWindowSystem::cleanup(state.glfwWindowData);
}
