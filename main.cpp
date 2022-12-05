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

    EngineConfig config {
        rendererConfig,
        windowConfig,
        LifecycleConfig {
            Lifecycle::init,
            Lifecycle::preRenderUpdate,
            Lifecycle::postRenderUpdate,
            Lifecycle::cleanup
        }
    };

    EngineCore::run(config, nullptr);

    return 0;
}
