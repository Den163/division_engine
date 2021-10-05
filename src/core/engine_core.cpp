#include "engine_core.h"

#include <iostream>

#include "states/engine_state.h"
#include "systems/gl_shader_program_system.h"
#include "systems/glfw_window_system.h"
#include "systems/glfw_input_system.h"
#include "systems/render_tick_system.h"
#include "systems/gl_render_system.h"
#include "systems/check_gl_mesh_initialization_system.h"
#include "systems/glfw_vsync_system.h"
#include "systems/loop_tick_system.h"
#include "../hook/lifecycle.h"

static void init(EngineState& state);
static void mainLoop(EngineState& state);
static void cleanup(EngineState& state);

void EngineCore::run(WindowConfig& windowConfig, RendererConfig& rendererConfig)
{
    EngineState engineState {};
    engineState.windowConfig = windowConfig;
    engineState.rendererConfig = rendererConfig;

    init(engineState);
    mainLoop(engineState);
    cleanup(engineState);
}

static void init(EngineState& state)
{
    Lifecycle::preInit(state);

    LoopTickSystem::init(state.loopUpdateTimestamp);
    RenderTickSystem::init(state.rendererState);
    GlfWindowSystem::init(state.windowState, state.glfwWindowData, state.windowConfig);
    GlShaderProgramSystem::init(state.shaderState);
    GlRenderSystem::init(state.shaderState, state.windowConfig);

    Lifecycle::postInit(state);

#ifdef PRINT_OPENGL_INFO
    DebugUtils::printRendererInfo();
#endif
}

static void mainLoop(EngineState& state)
{
    const auto& rendererState = state.rendererState;
    const auto& windowState = state.windowState;

    do
    {
        LoopTickSystem::update(state.loopUpdateTimestamp);
        RenderTickSystem::update(state.rendererState, state.rendererConfig, state.loopUpdateTimestamp);

        Lifecycle::beginLoopUpdate(state);
        if (rendererState.shouldUpdate)
        {
            CheckGlMeshInitializationSystem::update(state.ecsRegistry, state.shaderState);
            Lifecycle::preRenderUpdate(state);
            GlRenderSystem::update(state.ecsRegistry, state.shaderState, state.rendererConfig);
            GlfwVSyncSystem::update(state.glfwWindowData);
            Lifecycle::postRenderUpdate(state);
        }

        GlfWindowSystem::update(state.windowState, state.glfwWindowData);
        GlfwInputSystem::update();
        Lifecycle::endLoopUpdate(state);
    }
    while (!windowState.shouldClose);
}

static void cleanup(EngineState& state)
{
    Lifecycle::preCleanup(state);
    GlShaderProgramSystem::cleanup(state.shaderState);
    GlfWindowSystem::cleanup(state.glfwWindowData);
    Lifecycle::postCleanup(state);
}

