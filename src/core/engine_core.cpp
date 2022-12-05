#include "engine_core.h"

static inline void init(EngineState&, const EngineConfig&);
static inline void eventLoop(EngineState&, const EngineConfig&);
static inline void renderLoop(EngineState&, const EngineConfig&);
static inline void cleanup(EngineState&, const EngineConfig&);

void EngineCore::run(const EngineConfig &config, void *userState) {
    EngineState state;
    state.userState = userState;

    init(state, config);
    eventLoop(state, config);
    cleanup(state, config);
}


static void init(EngineState& state, const EngineConfig& config)
{
    PlatformRegisterInputSystem::init(state);
    RegisterInputSystem::init(state);
    LoopTickSystem::init(state);
    RenderTickSystem::init(state, config);
    PlatformWindowSystem::init(state, config);
    GlShaderProgramSystem::init(state, config);

    config.lifeCycle.init(state);

#ifdef PRINT_OPENGL_INFO
    DebugUtils::printRendererInfo();
#endif
}

void eventLoop(EngineState& state, const EngineConfig& config)
{
    do
    {
        LoopTickSystem::update(state);
        RenderTickSystem::update(state);
        PlatformRegisterInputSystem::update(state);
        RegisterInputSystem::eventLoop(state);

        if (state.renderer.shouldUpdate)
        {
            renderLoop(state, config);
        }

        PlatformWindowSystem::update(state);
    }
    while (!state.window.shouldClose);
}

static void renderLoop(EngineState& state, const EngineConfig& config)
{
    config.lifeCycle.preRender(state);
    OnGuiMeshEntityCreatedEventSystem::preRender(state);
    OnGuiMeshEntityDestroyedEventSystem::preRender(state);
    DebugRenderPassSystem::preRender(state);

    GlPrepareFramebufferSystem::update(state);

    GlGuiTextVertexSystem::update(state);
    GlGuiRectVertexSystem::update(state);

    GlMvpMatrixSystem::update(state);
    GlDrawGuiMeshSystem::update(state);

    PlatformVSyncSystem::update(state);

    DebugRenderPassSystem::postRender(state);
    config.lifeCycle.postRender(state);
    RegisterInputSystem::postRender(state);

    state.guiRegistry.clear<GuiMeshCreateEvent, GuiMeshDestroyed>();
}

static void cleanup(EngineState& state, const EngineConfig& config)
{
    config.lifeCycle.cleanup(state);
    GlShaderProgramSystem::cleanup(state);
    PlatformWindowSystem::cleanup(state);
}
