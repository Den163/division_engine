#define GLFW_INCLUDE_NONE

#include "engine_core.h"

#include <iostream>

#include "engine_state.h"
#include "systems/gl_shader_program_system.h"
#include "systems/glfw_window_system.h"
#include "systems/glfw_input_system.h"
#include "systems/render_tick_system.h"
#include "systems/gl_render_system.h"
#include "systems/glfw_vsync_system.h"
#include "systems/loop_tick_system.h"

static void init(EngineState& engineState);
static void mainLoop(EngineState& engineState);
static void cleanup(EngineState& engineState);

void EngineCore::run(WindowConfig& windowConfig, RendererConfig& rendererConfig)
{
    EngineState engineState {};
    engineState.windowConfig = windowConfig;
    engineState.rendererConfig = rendererConfig;

    init(engineState);
    mainLoop(engineState);
    cleanup(engineState);
}

static void init(EngineState& engineState)
{
    LoopTickSystem::init(engineState.loopUpdateTimestamp);
    RenderTickSystem::init(engineState.rendererState);
    GlfWindowSystem::init(engineState.windowState, engineState.glfwWindowData, engineState.windowConfig);
    GlShaderProgramSystem::init(engineState.shaderState);
    GlRenderSystem::init(engineState.shaderState, engineState.windowConfig);

#ifdef PRINT_OPENGL_INFO
    DebugUtils::printRendererInfo();
#endif
}

static void mainLoop(EngineState& engineState)
{
    const auto& rendererState = engineState.rendererState;
    const auto& windowState = engineState.windowState;

    do
    {
        LoopTickSystem::update(engineState.loopUpdateTimestamp);
        RenderTickSystem::update(engineState.rendererState, engineState.rendererConfig, engineState.loopUpdateTimestamp);
        if (rendererState.shouldUpdate)
        {
            GlRenderSystem::update(engineState.shaderState, engineState.rendererConfig);
            GlfwVSyncSystem::update(engineState.glfwWindowData);
        }

        GlfWindowSystem::update(engineState.windowState, engineState.glfwWindowData);
        GlfwInputSystem::update();

        if (windowState.shouldClose) return;
    }
    while (true);
}

static void cleanup(EngineState& engineState)
{
    GlShaderProgramSystem::cleanup(engineState.shaderState);
    GlfWindowSystem::cleanup(engineState.glfwWindowData);
}

