#include "render_system.h"

#include <chrono>
#include <glad/gl.h>
#include <iostream>

#include "../components/renderer_state.h"
#include "../components/window_config.h"

void RenderSystem::init(singleton_registry& engineData)
{
    auto& renderer = engineData.get<RendererState>();
    renderer.lastUpdateTime = std::chrono::steady_clock::now();

    auto& renderedFrameData = engineData.emplace<RenderedFrameData>();
    renderedFrameData.deltaTimeSeconds = 0;

    auto& windowCfg = engineData.get<WindowConfig>();
    glViewport(0, 0, windowCfg.width, windowCfg.height);
}

void RenderSystem::update(singleton_registry& engineData)
{
    auto& rendererState = engineData.get<RendererState>();
    rendererState.lastUpdateTime = std::chrono::steady_clock::now();
}

void RenderSystem::cleanup(singleton_registry& engineData)
{

}
