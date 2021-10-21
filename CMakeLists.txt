cmake_minimum_required(VERSION 3.20)
project(division_engine)

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

#LIBS

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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

include_directories(${CMAKE_SOURCE_DIR}/libs/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/glfw/include)
link_directories(${CMAKE_SOURCE_DIR}/libs/glfw)

add_library(glad libs/glad/src/gl.c)

#SHADERS

if(NOT DEFINED ENV{GLSL_LANG_VALIDATOR_COMP})
    message(FATAL_ERROR
            "Add environment variable GLSL_LANG_VALIDATOR_COMP with path to glslangValidator compiler"
            "(including executable). If it's not works try to reboot the system")
endif()

set(SHADER_SOURCES
    shaders/default_gui.vert
    shaders/default_gui.frag
)

foreach(SHADER IN LISTS SHADER_SOURCES)
    set(SHADER_INPUT ${CMAKE_SOURCE_DIR}/${SHADER})
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
    src/utils/debug_utils.cpp src/utils/debug_utils.h
    src/core/states/engine_state.h
    src/core/engine_core.cpp src/core/engine_core.h
    src/core/systems/rendering/gl_shader_program_system.cpp src/core/systems/rendering/gl_shader_program_system.h
    src/utils/file_utils.cpp src/utils/file_utils.h
    src/core/systems/window/glfw_window_system.cpp src/core/systems/window/glfw_window_system.h
    src/core/systems/rendering/gl_render_gui_system.cpp src/core/systems/rendering/gl_render_gui_system.h
    src/core/states/gl_shader_pipeline_state.h
    src/core/states/glfw_window_state.h
    src/core/states/window_state.h
    src/core/states/renderer_state.h
    src/core/configs/renderer_config.h
    src/core/configs/window_config.h
    src/core/systems/rendering/render_tick_system.cpp src/core/systems/rendering/render_tick_system.h
    src/core/systems/window/glfw_vsync_system.cpp src/core/systems/window/glfw_vsync_system.h
    src/core/states/update_time_state.h
    src/core/systems/loop_tick_system.cpp src/core/systems/loop_tick_system.h
    src/core/components/gl_mesh.h
    src/core/components/gui_mesh.h
    src/core/systems/input/win32_register_input_system.cpp src/core/systems/input/win32_register_input_system.h
    src/core/states/input_state.h
    src/core/states/raw_input_state.h
    src/core/states/keyboard_state.h
    src/core/systems/input/register_input_system.cpp src/core/systems/input/register_input_system.h
    src/utils/math.h
    src/core/components/rotation.h
    src/core/components/position.h
    src/core/components/scale.h
    src/core/states/camera_state.h
    src/core/primitives/gui/gui_primitive_factory.cpp src/core/primitives/gui/gui_primitive_factory.h
    src/core/components/key_code.h
    src/core/states/win32_state.h
    src/core/systems/window/win32_window_system.cpp src/core/systems/window/win32_window_system.h
    src/core/primitives/gui/gui_triangle.h src/core/primitives/gui/gui_triangle.cpp
    src/core/components/transform.h
    src/utils/color.h
    src/core/primitives/gui/gui_quad.h src/core/primitives/gui/gui_quad.cpp
    src/core/configs/engine_config.h src/core/configs/shader_config.h src/core/configs/lifecycle_config.h
    src/utils/memory.h src/utils/gl_utils.h
    src/core/events/create_gui_mesh_entity.h src/core/events/destroy_gui_mesh_entity.h
    src/core/event_systems/on_gui_mesh_entity_created_event_system.cpp
    src/core/event_systems/on_gui_mesh_entity_created_event_system.h
    src/core/event_systems/on_gui_mesh_entity_destroyed_event_system.cpp
    src/core/event_systems/on_gui_mesh_entity_destroyed_event_system.h
    src/core/systems/rendering/gl_prepare_framebuffer_system.cpp
    src/core/systems/rendering/gl_prepare_framebuffer_system.h src/core/configs/shader_pipeline_config.h src/core/states/gl_shader_state.h src/utils/engine_state_helper.h src/utils/engine_invariants.h)

set(DIVISION_ENGINE_LIBS glad glfw3 glm::glm EnTT::EnTT)

add_library(division_engine ${DIVISION_ENGINE_SOURCES})
add_dependencies(division_engine shaders)
target_link_libraries(division_engine ${DIVISION_ENGINE_LIBS})

#RUN

add_executable(division_engine_run main.cpp test_hook/lifecycle.cpp test_hook/lifecycle.h)
target_link_libraries(division_engine_run division_engine)

#TESTS

set(DIVISION_ENGINE_TESTS_SOURCES tests/colors_tests.cpp tests/test_sample.cpp)

add_executable(division_engine_tests ${DIVISION_ENGINE_TESTS_SOURCES})
target_link_libraries(division_engine_tests division_engine gtest gtest_main ${DIVISION_ENGINE_LIBS})
