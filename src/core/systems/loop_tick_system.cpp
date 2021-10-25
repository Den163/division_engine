#include "loop_tick_system.h"

void LoopTickSystem::init(EngineState& engineState)
{
    auto& loopUpdateTimestamp = engineState.loopUpdate;

    loopUpdateTimestamp.lastUpdateTime = std::chrono::steady_clock::now();
    loopUpdateTimestamp.deltaTime = std::chrono::duration<float> { 0 };
}

void LoopTickSystem::update(EngineState& engineState)
{
    auto& loopUpdateTimestamp = engineState.loopUpdate;
    const auto& now = std::chrono::steady_clock::now();

    loopUpdateTimestamp.deltaTime = now - loopUpdateTimestamp.lastUpdateTime;
    loopUpdateTimestamp.lastUpdateTime = now;
}
