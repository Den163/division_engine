cmake_minimum_required(VERSION 3.16)
project(division_engine_core)

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

find_program(GLSLANG_VALIDATOR glslangValidator)
if(NOT GLSLANG_VALIDATOR)
    message(FATAL_ERROR "Couldn't find the glslangValidator. Please add it to your PATH")
endif()

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

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.5
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
        GIT_TAG VER-2-11-0
)
FetchContent_MakeAvailable(freetype)

add_library(glad ${PROJECT_SOURCE_DIR}/libs/glad/src/gl.c)
target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/libs/glad/include)
add_library(stb_image libs/stb_image/stb_image.h libs/stb_image/stb_image.h.cpp)
target_include_directories(stb_image PUBLIC ${PROJECT_SOURCE_DIR}/libs/stb_image)
add_library(stb_image_write libs/stb_image_write/stb_image_write.h libs/stb_image_write/stb_image_write.cpp)
target_include_directories(stb_image_write PUBLIC libs/stb_image_write)

#SHADERS

set(SHADER_SOURCES
    shaders/default_gui.vert
    shaders/default_gui_colored.frag
    shaders/default_gui_texture.frag
    shaders/default_gui_font.frag
)

foreach(SHADER IN LISTS SHADER_SOURCES)
    set(SHADER_INPUT ${PROJECT_SOURCE_DIR}/${SHADER})
    set(SHADER_OUTPUT ${CMAKE_BINARY_DIR}/${SHADER}.spv)

    get_filename_component(SHADER_OUTPUT_DIR ${SHADER_OUTPUT} DIRECTORY)
    file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

    add_custom_command(
        OUTPUT ${SHADER_OUTPUT}
        COMMAND ${GLSLANG_VALIDATOR} -G100 -e main ${SHADER_INPUT} -o ${SHADER_OUTPUT}
        DEPENDS ${SHADER_INPUT}
        VERBATIM
    )

    list(APPEND SHADERS_OUTPUTS ${SHADER_OUTPUT})
endforeach()

add_custom_target(shaders DEPENDS ${SHADERS_OUTPUTS})

#ENGINE

set(DIVISION_ENGINE_SOURCES
    core/utils/debug_utils.cpp core/utils/debug_utils.h core/states/engine_state.h
    core/engine_core.h core/engine_core.cpp
    core/systems/rendering/gl_shader_program_system.cpp core/systems/rendering/gl_shader_program_system.h
    core/utils/file_utils.cpp core/utils/file_utils.h
    core/systems/window/platform/glfw_window_system.cpp core/systems/window/platform/glfw_window_system.h
    core/states/shader_pipeline_state.h core/states/platform/glfw_window_state.h core/states/window_state.h
    core/states/renderer_state.h core/configs/renderer_config.h core/configs/window_config.h
    core/systems/rendering/render_tick_system.cpp core/systems/rendering/render_tick_system.h
    core/systems/window/platform/glfw_vsync_system.cpp core/systems/window/platform/glfw_vsync_system.h
    core/systems/loop_tick_system.cpp core/systems/loop_tick_system.h
    core/states/update_time_state.h core/components/gl_mesh.h core/components/gui_mesh.h
    core/systems/input/platform/win32_register_input_system.cpp core/systems/input/platform/win32_register_input_system.h
    core/states/input_state.h core/states/raw_input_state.h core/states/keyboard_state.h
    core/systems/input/register_input_system.cpp core/systems/input/register_input_system.h
    core/utils/math.h core/components/rotation.h core/components/position.h core/components/scale.h
    core/states/camera_state.h core/components/key_code.h core/states/platform/win32_state.h
    core/systems/window/platform/win32_window_system.cpp core/systems/window/platform/win32_window_system.h
    core/configs/engine_config.h core/configs/shader_config.h core/configs/lifecycle_config.h
    core/utils/memory.h core/utils/gl_utils.h core/components/transform.h core/utils/color.h
    core/events/gui_mesh_create_event.h core/events/gui_mesh_destroy_event.h
    core/event_systems/on_gui_mesh_entity_created_event_system.cpp
    core/event_systems/on_gui_mesh_entity_created_event_system.h
    core/event_systems/on_gui_mesh_entity_destroyed_event_system.cpp
    core/event_systems/on_gui_mesh_entity_destroyed_event_system.h
    core/systems/rendering/gl_prepare_framebuffer_system.cpp core/systems/rendering/gl_prepare_framebuffer_system.h
    core/configs/shader_pipeline_config.h core/states/shader_state.h core/utils/engine_state_helper.h
    core/utils/engine_invariants.h core/configs/color_mode.h core/states/texture2d_state.h
    core/components/gl_texture.h core/configs/default_shaders_config.h core/states/default_shaders_state.h
    core/utils/shader_compiler.cpp core/utils/shader_compiler.h
    core/utils/texture_utils.cpp core/utils/texture_utils.h
    core/primitives/gui/font.h core/primitives/gui/glyph.h core/utils/font_utils.cpp core/utils/font_utils.h
    core/configs/texture_2d_config.h core/data_structures/sparse_set.h core/data_structures/versioned_sparse_set.h
    core/utils/disable_glm_warnings.h core/states/resources_state.h core/components/gui_text.h
    core/systems/rendering/gl_gui_text_vertex_system.cpp core/systems/rendering/gl_gui_text_vertex_system.h
    core/components/gui_material.h core/gui_composer/gui_composer.cpp core/gui_composer/gui_composer.h
    core/systems/rendering/gl_mvp_matrix_system.cpp core/systems/rendering/gl_mvp_matrix_system.h
    core/systems/rendering/gl_draw_gui_mesh_system.cpp core/systems/rendering/gl_draw_gui_mesh_system.h
    core/utils/gl_utils.cpp core/components/gui_rect.h core/components/gl_draw_indirect_command.h
    core/systems/rendering/gl_gui_rect_vertex_system.h core/systems/rendering/gl_gui_rect_vertex_system.cpp
    core/components/gui_rect_color.h core/systems/rendering/debug_render_pass_system.cpp
    core/systems/rendering/debug_render_pass_system.h core/states/platform_state.h
    core/systems/input/platform_register_input_system.cpp core/systems/input/platform_register_input_system.h
    core/systems/window/platform_window_system.cpp core/systems/window/platform_window_system.h
    core/systems/window/platform_vsync_system.cpp core/systems/window/platform_vsync_system.h)

set(DIVISION_ENGINE_PLATFORM_LIBS)
set(DIVISION_ENGINE_LIBS glad freetype glm::glm EnTT::EnTT stb_image stb_image_write)

add_library(division_engine_core ${DIVISION_ENGINE_SOURCES})
add_dependencies(division_engine_core shaders)

if (WIN32 OR UNIX)
    target_compile_definitions(division_engine_core PUBLIC USE_GLFW)
    list(APPEND DIVISION_ENGINE_PLATFORM_LIBS glfw)
endif()

target_include_directories(division_engine_core PUBLIC core)
target_link_libraries(division_engine_core ${DIVISION_ENGINE_LIBS} ${DIVISION_ENGINE_PLATFORM_LIBS})

target_compile_definitions(division_engine_core PUBLIC GLM_FORCE_SWIZZLE GLM_FORCE_CXX17)

if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    if (MSVC)
        target_compile_options(division_engine_core PUBLIC /EHs)
    endif()
endif()