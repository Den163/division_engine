cmake_minimum_required(VERSION 3.20)
project(division_engine)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
add_subdirectory(src)

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
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.6.0
)
FetchContent_MakeAvailable(benchmark)

#RUN
add_executable(division_engine_run main.cpp test_hook/lifecycle.cpp test_hook/lifecycle.h test_hook/global_state.h)
target_link_libraries(division_engine_run division_engine_core)

add_custom_command(TARGET division_engine_run PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
                   DEPENDS ${CMAKE_BINARY_DIR}/assets
)

#TESTS

set(DIVISION_ENGINE_TESTS_SOURCES tests/colors_tests.cpp tests/test_sample.cpp tests/sparse_set_tests.cpp)

add_executable(division_engine_tests ${DIVISION_ENGINE_TESTS_SOURCES})
target_link_libraries(division_engine_tests division_engine_core gtest gtest_main)

#BENCHMARKS

set(DIVISION_ENGINE_BENCHMARKS_SOURCES tests_benchmarks/sample_benchmark.cpp)

add_executable(division_engine_benchmarks ${DIVISION_ENGINE_BENCHMARKS_SOURCES})
target_link_libraries(division_engine_benchmarks division_engine_core benchmark::benchmark)
