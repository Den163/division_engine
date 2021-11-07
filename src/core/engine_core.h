#pragma once

#include "utils/disable_glm_warnings.h"

#include "configs/engine_config.h"
#include "event_systems/on_gui_mesh_entity_created_event_system.h"
#include "event_systems/on_gui_mesh_entity_destroyed_event_system.h"
#include "systems/rendering/gl_shader_program_system.h"
#include "systems/rendering/gl_prepare_framebuffer_system.h"
#include "systems/rendering/render_tick_system.h"
#include "systems/rendering/gl_gui_text_vertex_system.h"
#include "systems/loop_tick_system.h"
#include "systems/input/register_input_system.h"
#include "events/gui_mesh_create_event.h"
#include "events/gui_mesh_destroy_event.h"
#include "systems/rendering/gl_mvp_matrix_system.h"
#include "systems/rendering/gl_draw_gui_mesh_system.h"
#include "systems/rendering/gl_gui_rect_vertex_system.h"
#include "systems/rendering/debug_render_pass_system.h"
#include "systems/input/platform_register_input_system.h"
#include "systems/window/platform_window_system.h"
#include "systems/window/platform_vsync_system.h"

#include <type_traits>

class EngineCore
{
public:
    template<typename TState>
    static inline void run(TState& state)
    {
        init(state);
        eventLoop(state);
        cleanup(state);
    }

private:
    template<typename TState>
    static inline void init(TState& globalState)
    {
        EngineState& state = globalState.engineState;
        EngineConfig& config = globalState.engineConfig;

        PlatformRegisterInputSystem::init(state);
        RegisterInputSystem::init(state);
        LoopTickSystem::init(state);
        RenderTickSystem::init(state, config);
        PlatformWindowSystem::init(state, config);
        GlShaderProgramSystem::init(state, config);

        globalState.lifecycle.init(globalState);

#ifdef PRINT_OPENGL_INFO
        DebugUtils::printRendererInfo();
#endif
    }

    template<typename TState>
    static inline void eventLoop(TState& globalState)
    {
        EngineState& state = globalState.engineState;
        const EngineConfig& config = globalState.engineConfig;

        do
        {
            LoopTickSystem::update(state);
            RenderTickSystem::update(state);
            PlatformRegisterInputSystem::update(state);
            RegisterInputSystem::eventLoop(state);

            if (state.renderer.shouldUpdate)
            {
                renderLoop(globalState);
            }

            PlatformWindowSystem::update(state);
        }
        while (!state.window.shouldClose);
    }

    template<typename TState>
    static inline void renderLoop(TState& globalState)
    {
        EngineState& state = globalState.engineState;

        globalState.lifecycle.preRender(globalState);
        OnGuiMeshEntityCreatedEventSystem::preRender(state);
        OnGuiMeshEntityDestroyedEventSystem::preRender(state);
        DebugRenderPassSystem::preRender(state);

        GlPrepareFramebufferSystem::update(state);

        GlGuiTextVertexSystem::update(state);
        GlGuiRectVertexSystem::update(state);

        GlMvpMatrixSystem::update(state);
        GlDrawGuiMeshSystem::update(state);

        PlatformVSyncSystem::update(state);

        DebugRenderPassSystem::postRender(state);
        globalState.lifecycle.postRender(globalState);
        RegisterInputSystem::postRender(state);

        state.guiRegistry.clear<GuiMeshCreateEvent, GuiMeshDestroyed>();
    }

    template<typename TState>
    static inline void cleanup(TState& globalState)
    {
        EngineState& state = globalState.engineState;

        globalState.lifecycle.cleanup(globalState);
        GlShaderProgramSystem::cleanup(state);
        PlatformWindowSystem::cleanup(state);
    }
};