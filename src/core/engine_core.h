#pragma once

#include "utils/disable_glm_warnings.h"

#include <configs/engine_config.h>
#include <event_systems/on_gui_mesh_entity_created_event_system.h>
#include <event_systems/on_gui_mesh_entity_destroyed_event_system.h>
#include <systems/rendering/gl_shader_program_system.h>
#include <systems/rendering/gl_prepare_framebuffer_system.h>
#include <systems/rendering/render_tick_system.h>
#include <systems/rendering/gl_gui_text_vertex_system.h>
#include <systems/loop_tick_system.h>
#include <systems/input/register_input_system.h>
#include <events/gui_mesh_create_event.h>
#include <events/gui_mesh_destroy_event.h>
#include <systems/rendering/gl_mvp_matrix_system.h>
#include <systems/rendering/gl_draw_gui_mesh_system.h>
#include <systems/rendering/gl_gui_rect_vertex_system.h>
#include <systems/rendering/debug_render_pass_system.h>
#include <systems/input/platform_register_input_system.h>
#include <systems/window/platform_window_system.h>
#include <systems/window/platform_vsync_system.h>

#include <type_traits>

namespace EngineCore
{
    void run(const EngineConfig& config, void* userState);
};