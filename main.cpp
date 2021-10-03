#include "src/core/engine_core.h"

int main()
{
    WindowConfig windowConfig {};
    windowConfig.title = "GL UI";
    windowConfig.width = 640;
    windowConfig.height = 480;

    RendererConfig rendererConfig {};
    rendererConfig.backgroundColor = { 1, 0, 0, 1 };
    rendererConfig.targetFps = 60;

    EngineCore::run(windowConfig, rendererConfig);

    return 0;
}
