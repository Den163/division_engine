#include "src/core/engine_core.h"
#include "test_hook/lifecycle.h"

int main()
{
    WindowConfig windowConfig {};
    windowConfig.title = "GL UI";
    windowConfig.width = 1280;
    windowConfig.height = 960;

    RendererConfig rendererConfig {};
    rendererConfig.backgroundColor = { 0, 0, 0, 1 };
    rendererConfig.targetFps = 60;

    std::vector<ShaderConfig> shaders
    {
        ShaderConfig { "basic.vert", ShaderType::Vertex },
        ShaderConfig { "basic.frag", ShaderType::Fragment },
    };

    LifecycleConfig lifecycle
    {
        Lifecycle::init,
        Lifecycle::preRenderUpdate,
        Lifecycle::postRenderUpdate,
        Lifecycle::cleanup
    };

    const EngineConfig& config
    {
            rendererConfig,
            windowConfig,
            lifecycle,
            shaders,
    };
    EngineCore::run(config);

    return 0;
}
