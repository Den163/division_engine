#pragma once

#include "utils/disable_glm_warnings.h"

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

#include "configs/engine_config.h"
#include "event_systems/on_gui_mesh_entity_created_event_system.h"
#include "event_systems/on_gui_mesh_entity_destroyed_event_system.h"
#include "systems/rendering/gl_shader_program_system.h"
#include "systems/rendering/gl_prepare_framebuffer_system.h"
#include "systems/window/glfw_window_system.h"
#include "systems/rendering/render_tick_system.h"
#include "systems/rendering/gl_gui_text_vertex_system.h"
#include "systems/window/glfw_vsync_system.h"
#include "systems/loop_tick_system.h"
#include "systems/input/win32_register_input_system.h"
#include "systems/input/register_input_system.h"
#include "systems/window/win32_window_system.h"
#include "events/gui_mesh_create_event.h"
#include "events/gui_mesh_destroy_event.h"
#include "systems/rendering/gl_mvp_matrix_system.h"
#include "systems/rendering/gl_draw_gui_mesh_system.h"
#include "systems/rendering/gl_gui_rect_vertex_system.h"

#include <type_traits>

template<typename T>
concept EngineStateAggregate = requires(T aggr)
{
    { std::is_same_v< std::remove_reference_t<decltype(aggr.engineState)>, EngineState > };
    { std::is_same_v< std::remove_reference_t<decltype(aggr.engineConfig)>, EngineConfig > };
    { std::is_same_v< std::remove_reference_t<decltype(aggr.lifecycle)>, LifecycleConfig<T> > };
};

class EngineCore
{
public:
    template<EngineStateAggregate TState>
    static inline void run(TState& state)
    {
        init(state);
        eventLoop(state);
        cleanup(state);
    }

private:
    template<EngineStateAggregate TState>
    static inline void init(TState& globalState)
    {
        EngineState& state = globalState.engineState;
        EngineConfig& config = globalState.engineConfig;

        Win32RegisterInputSystem::init(state);
        RegisterInputSystem::init(state);
        LoopTickSystem::init(state);
        RenderTickSystem::init(state, config);
        GlfWindowSystem::init(state, config);
        Win32WindowSystem::init(state);
        GlShaderProgramSystem::init(state, config);

        globalState.lifecycle.init(globalState);

#ifdef PRINT_OPENGL_INFO
        DebugUtils::printRendererInfo();
#endif
    }

    template<EngineStateAggregate TState>
    static inline void eventLoop(TState& globalState)
    {
        EngineState& state = globalState.engineState;
        const EngineConfig& config = globalState.engineConfig;

        do
        {
            LoopTickSystem::update(state);
            RenderTickSystem::update(state);
            Win32RegisterInputSystem::update(state);
            RegisterInputSystem::eventLoop(state);

            if (state.renderer.shouldUpdate)
            {
                renderLoop(globalState);
            }

            GlfWindowSystem::update(state);
        }
        while (!state.window.shouldClose);
    }

    template<EngineStateAggregate TState>
    static inline void renderLoop(TState& globalState)
    {
        EngineState& state = globalState.engineState;

        globalState.lifecycle.preRender(globalState);
        OnGuiMeshEntityCreatedEventSystem::preRender(state);
        OnGuiMeshEntityDestroyedEventSystem::preRender(state);

        GlPrepareFramebufferSystem::update(state);

        GlGuiTextVertexSystem::update(state);
        GlGuiRectVertexSystem::update(state);

        GlMvpMatrixSystem::update(state);
        GlDrawGuiMeshSystem::update(state);

        GlfwVsyncSystem::update(state);

        globalState.lifecycle.postRender(globalState);
        RegisterInputSystem::postRender(state);

        state.guiRegistry.clear<GuiMeshCreateEvent, GuiMeshDestroyed>();
    }

    template<EngineStateAggregate TState>
    static inline void cleanup(TState& globalState)
    {
        EngineState& state = globalState.engineState;

        globalState.lifecycle.cleanup(globalState);
        GlShaderProgramSystem::cleanup(state);
        GlfWindowSystem::cleanup(state);
    }
};