#include "post_render_tick_system.h"

#include <chrono>

#include "../components/renderer_state.h"

void PostRenderTickSystem::init(singleton_registry& engineData)
{
    auto& renderer = engineData.get<RendererState>();
    renderer.lastUpdateTime = std::chrono::steady_clock::now();
}

void PostRenderTickSystem::update(singleton_registry& engineData)
{
    auto& rendererState = engineData.get<RendererState>();
    rendererState.lastUpdateTime = std::chrono::steady_clock::now();
}

void PostRenderTickSystem::cleanup(singleton_registry& engineData)
{

}
