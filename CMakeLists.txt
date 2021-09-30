cmake_minimum_required(VERSION 3.20)
project(gl_ui)

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
)

FetchContent_Declare(
        EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(EnTT)

include_directories(${CMAKE_SOURCE_DIR}/libs/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/glfw/include)
link_directories(${CMAKE_SOURCE_DIR}/libs/glfw)

if (NOT DEFINED ENV{GLSL_LANG_VALIDATOR_COMP})
    message(FATAL_ERROR
            "Add environment variable GLSL_LANG_VALIDATOR_COMP with path to glslangValidator compiler"
            "(including executable). If it's not works try to reboot the system"
            )
endif ()

set(SHADER_SOURCES
    basic.vert
    basic.frag
    )

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)

foreach (SHADER IN LISTS SHADER_SOURCES)
    set(SHADER_INPUT ${CMAKE_SOURCE_DIR}/shaders/${SHADER})
    set(SHADER_OUTPUT ${CMAKE_BINARY_DIR}/shaders/${SHADER}.spv)

    add_custom_command(
            OUTPUT ${SHADER_OUTPUT}
            COMMAND $ENV{GLSL_LANG_VALIDATOR_COMP}/glslangValidator.exe -G100 -e main ${SHADER_INPUT} -o ${SHADER_OUTPUT}
            DEPENDS ${SHADER_INPUT}
            VERBATIM
    )

    list(APPEND SHADERS_OUTPUTS ${SHADER_OUTPUT})
endforeach ()

add_custom_target(shaders DEPENDS ${SHADERS_OUTPUTS})

set(SOURCES
    main.cpp
    libs/glad/src/gl.c
    src/utils/debug_utils.cpp
    src/utils/debug_utils.h
    src/core/engine_core.cpp
    src/core/engine_core.h
    src/core/systems/shader_program_system.cpp
    src/core/systems/shader_program_system.h
    src/utils/file_utils.cpp
    src/utils/file_utils.h
    src/core/systems/window_system.cpp
    src/core/systems/window_system.h
    src/core/systems/input_system.cpp
    src/core/systems/input_system.h
    src/core/systems/render_system.cpp
    src/core/systems/render_system.h
    src/utils/singleton_registry.h
    src/core/components/gl_shader_program.h
    src/core/components/glfw_window_data.h
    src/core/components/window_state.h
    src/core/components/renderer_state.h
    src/core/components/rendered_frame_state.h
    src/core/components/renderer_config.h
    src/core/components/window_config.h
    src/core/systems/render_tick_system.cpp
    src/core/systems/render_tick_system.h
    src/core/systems/vsync_system.cpp
    src/core/systems/vsync_system.h
)

add_executable(gl_ui ${SOURCES})
add_dependencies(gl_ui shaders)
target_link_libraries(gl_ui glfw3 glm::glm EnTT::EnTT)