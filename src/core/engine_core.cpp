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

static void init(EngineState& state, const EngineConfig& config);
static void eventLoop(EngineState& state, const EngineConfig& config);
static void renderLoop(EngineState& state, const EngineConfig& engineConfig);
static void cleanup(EngineState& state, const EngineConfig& engineConfig);

void EngineCore::run(const EngineConfig& engineConfig)
{
    EngineState engineState {};

    init(engineState, engineConfig);
    eventLoop(engineState, engineConfig);
    cleanup(engineState, engineConfig);
}

static void init(EngineState& state, const EngineConfig& config)
{
    Win32RegisterInputSystem::init(state);
    RegisterInputSystem::init(state);
    LoopTickSystem::init(state);
    RenderTickSystem::init(state, config);
    GlfWindowSystem::init(state, config);
    Win32WindowSystem::init(state);
    GlShaderProgramSystem::init(state, config);
    GlRenderGuiSystem::init(state);

    config.lifecycle.init(state);

#ifdef PRINT_OPENGL_INFO
    DebugUtils::printRendererInfo();
#endif
}

static void eventLoop(EngineState& state, const EngineConfig& config)
{
    const auto& rendererState = state.rendererState;
    const auto& windowState = state.windowState;

    do
    {
        LoopTickSystem::update(state);
        RenderTickSystem::update(state);
        Win32RegisterInputSystem::update(state);
        RegisterInputSystem::eventLoop(state);

        if (rendererState.shouldUpdate)
        {
            renderLoop(state, config);
        }

        GlfWindowSystem::update(state);
    }
    while (!windowState.shouldClose);
}

void renderLoop(EngineState& state, const EngineConfig& engineConfig)
{
    engineConfig.lifecycle.preRender(state);

    OnGuiMeshEntityCreatedEventSystem::update(state);
    OnGuiMeshEntityDestroyedEventSystem::update(state);

    GlPrepareFramebufferSystem::update(state);
    GlRenderGuiSystem::update(state);
    GlfwVsyncSystem::update(state);

    engineConfig.lifecycle.postRender(state);
    RegisterInputSystem::postRender(state);
}

static void cleanup(EngineState& state, const EngineConfig& engineConfig)
{
    engineConfig.lifecycle.cleanup(state);
    GlShaderProgramSystem::cleanup(state);
    GlfWindowSystem::cleanup(state);
}
