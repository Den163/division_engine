#include "render_tick_system.h"
#include "../../states/engine_state.h"
#include "../../configs/engine_config.h"

#include <chrono>

void RenderTickSystem::init(EngineState& engineState, const EngineConfig& engineConfig)
{
    auto& rendererState = engineState.rendererState;
    const auto& rendererConfig = engineConfig.renderer;

    rendererState.backgroundColor = rendererConfig.backgroundColor;
    rendererState.targetFps = rendererConfig.targetFps;

    rendererState.shouldUpdate = false;
    rendererState.updateTime.lastUpdateTime = std::chrono::steady_clock::now();
    rendererState.updateTime.deltaTime = std::chrono::duration<float> { 0 };
}

void RenderTickSystem::update(EngineState& engineState)
{
    auto& rendererState = engineState.rendererState;
    auto& updateTimeStamp = rendererState.updateTime;

    const auto& loopUpdateTimestamp = engineState.loopUpdateTimestamp;
    const auto& deltaTimeToUpdate = std::chrono::duration<float> { 1 / rendererState.targetFps };
    const auto& now = loopUpdateTimestamp.lastUpdateTime;
    const auto& deltaTime = now - updateTimeStamp.lastUpdateTime;

    auto shouldUpdate = deltaTime >= deltaTimeToUpdate;
    rendererState.shouldUpdate = shouldUpdate;

    if (shouldUpdate)
    {
        updateTimeStamp.lastUpdateTime = now;
        updateTimeStamp.deltaTime = deltaTime;
    }
}