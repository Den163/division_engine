#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

#include "engine_core.h"

#include "event_systems/on_gui_mesh_entity_created_event_system.h"
#include "event_systems/on_gui_mesh_entity_destroyed_event_system.h"
#include "events/gui_mesh_created.h"
#include "events/gui_mesh_destroyed.h"
#include "systems/rendering/gl_shader_program_system.h"
#include "systems/rendering/gl_prepare_framebuffer_system.h"
#include "systems/window/glfw_window_system.h"
#include "systems/rendering/render_tick_system.h"
#include "systems/rendering/gl_gui_mesh_vertex_system.h"
#include "systems/window/glfw_vsync_system.h"
#include "systems/loop_tick_system.h"
#include "systems/input/win32_register_input_system.h"
#include "systems/input/register_input_system.h"
#include "systems/window/win32_window_system.h"
#include "systems/rendering/gl_render_gui_mesh_system.h"
#include "systems/rendering/gl_texture2d_system.h"

static inline void init(EngineState& state, const EngineConfig& config);
static inline void eventLoop(EngineState& state, const EngineConfig& config);
static inline void renderLoop(EngineState& state, const EngineConfig& engineConfig);
static inline void cleanEvents(EngineState& state);
static inline void cleanup(EngineState& state, const EngineConfig& engineConfig);

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
    GlGuiMeshVerticesSystem::init(state);
    GlTexture2dSystem::init(state, config);

    config.lifecycle.init(state);

#ifdef PRINT_OPENGL_INFO
    DebugUtils::printRendererInfo();
#endif
}

static void eventLoop(EngineState& state, const EngineConfig& config)
{
    const auto& rendererState = state.renderer;
    const auto& windowState = state.window;

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
    OnGuiMeshEntityCreatedEventSystem::preRender(state);
    OnGuiMeshEntityDestroyedEventSystem::preRender(state);

    GlPrepareFramebufferSystem::update(state);
    GlGuiMeshVerticesSystem::update(state);
    GlTexture2dSystem::update(state);
    GlRenderGuiMeshSystem::update(state);

    GlfwVsyncSystem::update(state);

    engineConfig.lifecycle.postRender(state);
    RegisterInputSystem::postRender(state);
    cleanEvents(state);
}

static void cleanEvents(EngineState& state)
{
    state.guiRegistry.clear<GuiMeshCreated, GuiMeshDestroyed>();
}

static void cleanup(EngineState& state, const EngineConfig& engineConfig)
{
    engineConfig.lifecycle.cleanup(state);
    GlShaderProgramSystem::cleanup(state);
    GlfWindowSystem::cleanup(state);
}
