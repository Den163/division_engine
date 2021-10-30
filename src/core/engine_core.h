#pragma once

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

#include "configs/renderer_config.h"
#include "configs/window_config.h"
#include "configs/engine_config.h"
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
#include "systems/rendering/gl_gui_mesh_render_system.h"

#include <type_traits>

template<typename T>
concept EngineStateAggregate = requires(T& aggr)
{
    { aggr.engineState } -> std::same_as<EngineState&>;
    { aggr.engineConfig } -> std::same_as<EngineConfig&>;
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
        const LifecycleConfig<TState>& lifecycle = globalState.lifecycle;

        Win32RegisterInputSystem::init(state);
        RegisterInputSystem::init(state);
        LoopTickSystem::init(state);
        RenderTickSystem::init(state, config);
        GlfWindowSystem::init(state, config);
        Win32WindowSystem::init(state);
        GlShaderProgramSystem::init(state, config);

        lifecycle.init(globalState);

#ifdef PRINT_OPENGL_INFO
        DebugUtils::printRendererInfo();
#endif
    }

    template<EngineStateAggregate TState>
    static inline void eventLoop(TState& globalState)
    {
        EngineState& state = globalState.engineState;
        const EngineConfig& config = globalState.engineConfig;
        const LifecycleConfig<TState>& lifecycle = globalState.lifecycle;

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
                renderLoop(globalState);
            }

            GlfWindowSystem::update(state);
        }
        while (!windowState.shouldClose);
    }

    template<EngineStateAggregate TState>
    static inline void renderLoop(TState& globalState)
    {
        EngineState& state = globalState.engineState;
        const LifecycleConfig<TState>& lifecycle = globalState.lifecycle;

        lifecycle.preRender(globalState);
        OnGuiMeshEntityCreatedEventSystem::preRender(state);
        OnGuiMeshEntityDestroyedEventSystem::preRender(state);

        GlPrepareFramebufferSystem::update(state);
        GlGuiMeshVerticesSystem::update(state);
        GlRenderGuiMeshSystem::update(state);

        GlfwVsyncSystem::update(state);

        lifecycle.postRender(globalState);
        RegisterInputSystem::postRender(state);

        state.guiRegistry.clear<GuiMeshCreated, GuiMeshDestroyed>();
    }

    template<EngineStateAggregate TState>
    static inline void cleanup(TState& globalState)
    {
        EngineState& state = globalState.engineState;
        const LifecycleConfig<TState>& lifecycle = globalState.lifecycle;

        lifecycle.cleanup(globalState);
        GlShaderProgramSystem::cleanup(state);
        GlfWindowSystem::cleanup(state);
    }
};