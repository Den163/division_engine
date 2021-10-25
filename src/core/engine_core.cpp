#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

#include "engine_core.h"



static inline void renderLoop(EngineState& state, const EngineConfig& engineConfig);
static inline void cleanEvents(EngineState& state);

void EngineCore::__Impl::init(EngineState& state, const EngineConfig& config)
{
    Win32RegisterInputSystem::init(state);
    RegisterInputSystem::init(state);
    LoopTickSystem::init(state);
    RenderTickSystem::init(state, config);
    GlfWindowSystem::init(state, config);
    Win32WindowSystem::init(state);
    GlShaderProgramSystem::init(state, config);
    GlGuiMeshVerticesSystem::init(state);
    GlTexture2dSystem::init(state, config);

    config.lifecycle.init(state);

#ifdef PRINT_OPENGL_INFO
    DebugUtils::printRendererInfo();
#endif
}

void EngineCore::__Impl::eventLoop(EngineState& state, const EngineConfig& config)
{
    const auto& rendererState = state.renderer;
    const auto& windowState = state.window;

    do
    {
        LoopTickSystem::update(state);
        RenderTickSystem::update(state);
        Win32RegisterInputSystem::update(state);
        RegisterInputSystem::eventLoop(state);

        if (rendererState.shouldUpdate)
        {
            renderLoop(state, config);
        }

        GlfWindowSystem::update(state);
    }
    while (!windowState.shouldClose);
}

void renderLoop(EngineState& state, const EngineConfig& engineConfig)
{
    engineConfig.lifecycle.preRender(state);
    OnGuiMeshEntityCreatedEventSystem::preRender(state);
    OnGuiMeshEntityDestroyedEventSystem::preRender(state);

    GlPrepareFramebufferSystem::update(state);
    GlGuiMeshVerticesSystem::update(state);
    GlTexture2dSystem::update(state);
    GlRenderGuiMeshSystem::update(state);

    GlfwVsyncSystem::update(state);

    engineConfig.lifecycle.postRender(state);
    RegisterInputSystem::postRender(state);
    cleanEvents(state);
}

static void cleanEvents(EngineState& state)
{
    state.guiRegistry.clear<GuiMeshCreated, GuiMeshDestroyed>();
}

void EngineCore::__Impl::cleanup(EngineState& state, const EngineConfig& engineConfig)
{
    engineConfig.lifecycle.cleanup(state);
    GlShaderProgramSystem::cleanup(state);
    GlfWindowSystem::cleanup(state);
}
