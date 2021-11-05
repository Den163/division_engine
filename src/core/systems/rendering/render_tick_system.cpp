#include "render_tick_system.h"
#include "../../states/engine_state.h"
#include "../../configs/engine_config.h"

#include <chrono>

void RenderTickSystem::init(EngineState& engineState, const EngineConfig& engineConfig)
{
    auto& rendererState = engineState.renderer;
    const auto& rendererConfig = engineConfig.renderer;

    rendererState.backgroundColor = rendererConfig.backgroundColor;
    rendererState.targetFps = rendererConfig.targetFps;

    rendererState.shouldUpdate = false;
    rendererState.frameDelta.lastUpdateTime = std::chrono::steady_clock::now();
    rendererState.frameDelta.deltaTime = std::chrono::duration<float> { 0 };
}

void RenderTickSystem::update(EngineState& engineState)
{
    auto& rendererState = engineState.renderer;
    auto& frameDeltaTimeStep = rendererState.frameDelta;

    const auto& loopUpdateTimestamp = engineState.loopUpdate;
    const auto& deltaTimeToUpdate = std::chrono::duration<float> { 1 / rendererState.targetFps };
    const auto& now = loopUpdateTimestamp.lastUpdateTime;
    const auto& deltaTime = now - frameDeltaTimeStep.lastUpdateTime;

    auto shouldUpdate = deltaTime >= deltaTimeToUpdate;
    rendererState.shouldUpdate = shouldUpdate;

    if (shouldUpdate)
    {
        frameDeltaTimeStep.lastUpdateTime = now;
        frameDeltaTimeStep.deltaTime = deltaTime;
    }
}