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

    const auto STANDARD_VERTEX_SHADER_IDX = 0;
    const auto STANDARD_FRAGMENT_SHADER_IDX = 1;

    std::vector<ShaderConfig> shaders { 2 };
    shaders[STANDARD_VERTEX_SHADER_IDX] = ShaderConfig { "default_gui.vert", ShaderType::Vertex };
    shaders[STANDARD_FRAGMENT_SHADER_IDX] = ShaderConfig { "default_gui.frag", ShaderType::Fragment };

    ShaderPipelineConfig standardPipeline { 1 };

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
        standardPipeline,
        shaders,
    };
    EngineCore::run(config);

    return 0;
}
