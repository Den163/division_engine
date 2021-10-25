cmake_minimum_required(VERSION 3.20)
project(division_engine_core)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

#LIBS

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.8.1
)
FetchContent_MakeAvailable(EnTT)

add_library(glad ${PROJECT_SOURCE_DIR}/libs/glad/src/gl.c)
target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/libs/glad/include)
add_library(stb_image libs/stb_image/stb_image.h libs/stb_image/stb_image.h.cpp)
target_include_directories(stb_image PUBLIC ${PROJECT_SOURCE_DIR}/libs/stb_image)

#SHADERS

if(NOT DEFINED ENV{GLSL_LANG_VALIDATOR_COMP})
    message(FATAL_ERROR
            "Add environment variable GLSL_LANG_VALIDATOR_COMP with path to glslangValidator compiler"
            "(including executable). If it's not works try to reboot the system")
endif()

set(SHADER_SOURCES
    shaders/default_gui.vert
    shaders/default_gui_colored.frag
    shaders/default_gui_texture.frag
)

foreach(SHADER IN LISTS SHADER_SOURCES)
    set(SHADER_INPUT ${PROJECT_SOURCE_DIR}/${SHADER})
    set(SHADER_OUTPUT ${CMAKE_BINARY_DIR}/${SHADER}.spv)

    get_filename_component(SHADER_OUTPUT_DIR ${SHADER_OUTPUT} DIRECTORY)
    file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

    add_custom_command(
            OUTPUT ${SHADER_OUTPUT}
            COMMAND $ENV{GLSL_LANG_VALIDATOR_COMP}/glslangValidator.exe -G100 -e main ${SHADER_INPUT} -o ${SHADER_OUTPUT}
            DEPENDS ${SHADER_INPUT}
            VERBATIM
    )

    list(APPEND SHADERS_OUTPUTS ${SHADER_OUTPUT})
endforeach()

add_custom_target(shaders DEPENDS ${SHADERS_OUTPUTS})

#ENGINE

set(DIVISION_ENGINE_SOURCES
    core/utils/debug_utils.cpp core/utils/debug_utils.h
    core/states/engine_state.h
    core/engine_core.h
    core/systems/rendering/gl_shader_program_system.cpp core/systems/rendering/gl_shader_program_system.h
    core/utils/file_utils.cpp core/utils/file_utils.h
    core/systems/window/glfw_window_system.cpp core/systems/window/glfw_window_system.h
    core/systems/rendering/gl_gui_mesh_vertex_system.cpp core/systems/rendering/gl_gui_mesh_vertex_system.h
    core/states/shader_pipeline_state.h
    core/states/glfw_window_state.h
    core/states/window_state.h
    core/states/renderer_state.h
    core/configs/renderer_config.h
    core/configs/window_config.h
    core/systems/rendering/render_tick_system.cpp core/systems/rendering/render_tick_system.h
    core/systems/window/glfw_vsync_system.cpp core/systems/window/glfw_vsync_system.h
    core/states/update_time_state.h
    core/systems/loop_tick_system.cpp core/systems/loop_tick_system.h
    core/components/gl_mesh.h
    core/components/gui_mesh.h
    core/systems/input/win32_register_input_system.cpp core/systems/input/win32_register_input_system.h
    core/states/input_state.h
    core/states/raw_input_state.h
    core/states/keyboard_state.h
    core/systems/input/register_input_system.cpp core/systems/input/register_input_system.h
    core/utils/math.h
    core/components/rotation.h
    core/components/position.h
    core/components/scale.h
    core/states/camera_state.h
    core/primitives/gui/gui_primitive_factory.cpp core/primitives/gui/gui_primitive_factory.h
    core/components/key_code.h
    core/states/win32_state.h
    core/systems/window/win32_window_system.cpp core/systems/window/win32_window_system.h
    core/primitives/gui/gui_triangle.h core/primitives/gui/gui_triangle.cpp
    core/components/transform.h
    core/utils/color.h
    core/primitives/gui/gui_quad.h core/primitives/gui/gui_quad.cpp
    core/configs/engine_config.h core/configs/shader_config.h core/configs/lifecycle_config.h
    core/utils/memory.h core/utils/gl_utils.h
    core/events/gui_mesh_created.h core/events/gui_mesh_destroyed.h
    core/event_systems/on_gui_mesh_entity_created_event_system.cpp
    core/event_systems/on_gui_mesh_entity_created_event_system.h
    core/event_systems/on_gui_mesh_entity_destroyed_event_system.cpp
    core/event_systems/on_gui_mesh_entity_destroyed_event_system.h
    core/systems/rendering/gl_prepare_framebuffer_system.cpp
    core/systems/rendering/gl_prepare_framebuffer_system.h
    core/configs/shader_pipeline_config.h core/states/shader_state.h core/utils/engine_state_helper.h
    core/utils/engine_invariants.h
    core/configs/color_mode.h core/states/texture2d_state.h
    core/systems/rendering/gl_render_gui_mesh_system.cpp core/systems/rendering/gl_render_gui_mesh_system.h
    core/systems/rendering/gl_texture2d_system.cpp core/systems/rendering/gl_texture2d_system.h
    core/components/gl_texture.h core/configs/default_shaders_config.h core/states/default_shaders_state.h
    core/utils/shader_compiler.h core/utils/shader_compiler.cpp
    core/utils/texture_utils.cpp core/utils/texture_utils.h)

set(DIVISION_ENGINE_LIBS glad glfw3 glm::glm EnTT::EnTT stb_image)

add_library(division_engine_core ${DIVISION_ENGINE_SOURCES})
add_dependencies(division_engine_core shaders)
target_link_libraries(division_engine_core ${DIVISION_ENGINE_LIBS})

target_link_directories(division_engine_core PUBLIC ${PROJECT_SOURCE_DIR}/libs/glfw)
target_include_directories(division_engine_core PUBLIC ${PROJECT_SOURCE_DIR}/libs/glfw/include)

add_custom_command(TARGET division_engine_core PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${PROJECT_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}/assets)