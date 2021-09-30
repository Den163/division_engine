#include "render_tick_system.h"

#include <chrono>

#include "../components/renderer_config.h"
#include "../components/renderer_state.h"
#include "../components/rendered_frame_state.h"
#include "../components/window_state.h"

#include <iostream>

void RenderTickSystem::init(singleton_registry& engineData)
{
    auto& renderer = engineData.emplace<RendererState>();
    renderer.shouldUpdate = false;
}

void RenderTickSystem::update(singleton_registry& engineData)
{
    const auto& rendererConfig = engineData.get<RendererConfig>();

    auto [renderer, frameData, window] = engineData.get<RendererState, RenderedFrameData, WindowState>();
    const auto& lastUpdateTime = renderer.lastUpdateTime;

    auto now = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastUpdateTime);
    auto deltaTimeToUpdate = 1 / rendererConfig.targetFps;

    frameData.deltaTimeSeconds = deltaTime.count();
    renderer.shouldUpdate = frameData.deltaTimeSeconds >= deltaTimeToUpdate;
}

void RenderTickSystem::cleanup(singleton_registry& engineData)
{

}
