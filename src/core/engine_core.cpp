#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

#include "engine_core.h"

#include <iostream>

#include "event_systems/on_gui_mesh_entity_created_event_system.h"
#include "event_systems/on_gui_mesh_entity_destroyed_event_system.h"
#include "systems/rendering/gl_shader_program_system.h"
#include "systems/rendering/gl_prepare_framebuffer_system.h"
#include "systems/window/glfw_window_system.h"
#include "systems/rendering/render_tick_system.h"
#include "systems/rendering/gl_render_gui_system.h"
#include "systems/window/glfw_vsync_system.h"
#include "systems/loop_tick_system.h"
#include "systems/input/win32_register_input_system.h"
#include "systems/input/register_input_system.h"

#include "systems/window/win32_window_system.h"

static void init(EngineState& state, const EngineConfig& engineConfig);
static void eventLoop(EngineState& state, const EngineConfig& engineConfig);
static void renderLoop(EngineState& state, const EngineConfig& engineConfig);
static void cleanup(EngineState& state, const EngineConfig& engineConfig);

void EngineCore::run(const EngineConfig& engineConfig)
{
    EngineState engineState {};

    init(engineState, engineConfig);
    eventLoop(engineState, engineConfig);
    cleanup(engineState, engineConfig);
}

static void init(EngineState& state, const EngineConfig& engineConfig)
{
    Win32RegisterInputSystem::init(state.rawInputState);
    RegisterInputSystem::init(state.inputState);
    LoopTickSystem::init(state.loopUpdateTimestamp);
    RenderTickSystem::init(state.rendererState, engineConfig.renderer);
    GlfWindowSystem::init(state.windowState, state.glfwWindowState, engineConfig.window);
    Win32WindowSystem::init(state.win32State, state.glfwWindowState);
    GlShaderProgramSystem::init(state.shaderState, engineConfig.shaders);
    GlRenderGuiSystem::init(state.shaderState, state.cameraState, state.windowState);

    engineConfig.lifecycle.init(state);

#ifdef PRINT_OPENGL_INFO
    DebugUtils::printRendererInfo();
#endif
}

static void eventLoop(EngineState& state, const EngineConfig& engineConfig)
{
    const auto& rendererState = state.rendererState;
    const auto& windowState = state.windowState;

    do
    {
        LoopTickSystem::update(state.loopUpdateTimestamp);
        RenderTickSystem::update(state.rendererState, state.loopUpdateTimestamp);
        Win32RegisterInputSystem::update(state.rawInputState, state.win32State);
        RegisterInputSystem::eventLoop(state.inputState, state.rawInputState);

        if (rendererState.shouldUpdate)
        {
            renderLoop(state, engineConfig);
        }

        GlfWindowSystem::update(state.windowState, state.glfwWindowState);
    }
    while (!windowState.shouldClose);
}

void renderLoop(EngineState& state, const EngineConfig& engineConfig)
{
    engineConfig.lifecycle.preRender(state);

    OnGuiMeshEntityCreatedEventSystem::update(state.guiRegistry, state.shaderState);
    OnGuiMeshEntityDestroyedEventSystem::update(state.guiRegistry, state.shaderState);

    GlPrepareFramebufferSystem::update(state.rendererState, state.windowState);
    GlRenderGuiSystem::update(
        state.guiRegistry, state.shaderState, state.rendererState, state.cameraState, state.windowState);
    GlfwVsyncSystem::update(state.glfwWindowState);

    engineConfig.lifecycle.postRender(state);
    RegisterInputSystem::postRender(state.inputState, state.rawInputState);
}

static void cleanup(EngineState& state, const EngineConfig& engineConfig)
{
    engineConfig.lifecycle.cleanup(state);
    GlShaderProgramSystem::cleanup(state.shaderState);
    GlfWindowSystem::cleanup(state.glfwWindowState);
}
