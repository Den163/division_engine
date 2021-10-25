#pragma once

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
#include "systems/rendering/gl_render_gui_mesh_system.h"
#include "systems/rendering/gl_texture2d_system.h"

#include <type_traits>

namespace EngineCore
{
    namespace __Impl
    {
        void init(EngineState& state, const EngineConfig& config);
        void eventLoop(EngineState& state, const EngineConfig& config);
        void cleanup(EngineState& state, const EngineConfig& config);
    }

    template<typename TState>
    void run(TState& globalState)
    {
        auto& engineState = globalState.engineState;
        const auto& engineConfig = globalState.engineConfig;

        __Impl::init(engineState, engineConfig);
        __Impl::eventLoop(engineState, engineConfig);
        __Impl::cleanup(engineState, engineConfig);
    }
}