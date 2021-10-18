#include "src/core/engine_core.h"

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

    EngineCore::run(EngineConfig { rendererConfig, windowConfig, shaders });

    return 0;
}
