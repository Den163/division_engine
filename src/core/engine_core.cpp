#include "engine_core.h"

#include <iostream>

#include "systems/shader_program_system.h"
#include "systems/window_system.h"
#include "systems/input_system.h"
#include "systems/pre_render_tick_system.h"
#include "systems/profile_updates_system.h"
#include "systems/render_system.h"
#include "systems/vsync_system.h"

#include "components/renderer_config.h"
#include "components/renderer_state.h"
#include "components/window_config.h"

static void init(singleton_registry& engineData);
static void mainLoop(singleton_registry& engineData);
static void cleanup(singleton_registry& engineData);

void EngineCore::run()
{
    singleton_registry singletonRegistry {};

    init(singletonRegistry);
    mainLoop(singletonRegistry);
    cleanup(singletonRegistry);
}

static void init(singleton_registry& engineData)
{
    auto& rendererConfig = engineData.emplace<RendererConfig>();
    rendererConfig.targetFps = 60;
    rendererConfig.backgroundColor = { 0, 1, 1, 1 };

    auto& windowConfig = engineData.emplace<WindowConfig>();
    windowConfig.title = "GL UI";
    windowConfig.width = 640;
    windowConfig.height = 480;

    WindowSystem::init(engineData);
    ShaderProgramSystem::init(engineData);
    PreRenderTickSystem::init(engineData);
    RenderSystem::init(engineData);
    VSyncSystem::init(engineData);
    InputSystem::init();
    ProfileUpdatesSystem::init(engineData);

#ifdef PRINT_OPENGL_INFO
    DebugUtils::printRendererInfo();
#endif
}

static void mainLoop(singleton_registry& engineData)
{
    do
    {
        PreRenderTickSystem::update(engineData);

        const auto& renderState = engineData.get<RendererState>();
        if (renderState.shouldUpdate)
        {
            RenderSystem::update(engineData);
            VSyncSystem::update(engineData);
        }

        WindowSystem::update(engineData);
        InputSystem::update();
        ProfileUpdatesSystem::update(engineData);

        const auto& window = engineData.get<WindowState>();
        if (window.shouldClose) return;
    }
    while (true);
}

static void cleanup(singleton_registry& engineData)
{
    InputSystem::cleanup();
    ShaderProgramSystem::cleanup(engineData);
    VSyncSystem::cleanup(engineData);
    RenderSystem::cleanup(engineData);
    PreRenderTickSystem::cleanup(engineData);
    WindowSystem::cleanup(engineData);
    ProfileUpdatesSystem::cleanup(engineData);
}

