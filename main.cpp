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
        ShaderConfig { "default_gui.vert", ShaderType::Vertex },
        ShaderConfig { "default_gui_colored.frag", ShaderType::Fragment },
        ShaderConfig { "default_gui_texture.frag", ShaderType::Fragment }
    };

    ShaderPipelineConfig pipelineConfig { 1 };

    std::vector<Texture2dConfig> textures
    {
        Texture2dConfig { "assets/images/img.jpg" }
    };

    GlobalState globalState {
        EngineState {},
        EngineConfig {
            rendererConfig,
            windowConfig,
            pipelineConfig,
            shaders,
            textures
        },
        LifecycleConfig<GlobalState> {
            Lifecycle::init,
            Lifecycle::preRenderUpdate,
            Lifecycle::postRenderUpdate,
            Lifecycle::cleanup
        }
    };

    EngineCore::run(globalState);

    return 0;
}
