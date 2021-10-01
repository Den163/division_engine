#include "pre_render_tick_system.h"

#include <chrono>

#include "../components/renderer_config.h"
#include "../components/renderer_state.h"

#include <iostream>

void PreRenderTickSystem::init(singleton_registry& engineData)
{
    auto& renderer = engineData.emplace<RendererState>();
    renderer.shouldUpdate = false;
    renderer.lastUpdateTime = std::chrono::steady_clock::now();
    renderer.deltaTime = std::chrono::duration<float> { 0 };
}

void PreRenderTickSystem::update(singleton_registry& engineData)
{
    auto [renderer, rendererConfig] = engineData.get<RendererState, const RendererConfig>();
    const auto deltaTimeToUpdate = std::chrono::duration<float> { 1 / rendererConfig.targetFps };
    const auto now = std::chrono::steady_clock::now();
    const auto deltaTime = now - renderer.lastUpdateTime;

    renderer.shouldUpdate = deltaTime >= deltaTimeToUpdate;
    if (renderer.shouldUpdate)
    {
        renderer.lastUpdateTime = now;
        renderer.deltaTime = deltaTime;
    }
}

void PreRenderTickSystem::cleanup(singleton_registry& engineData)
{
}
