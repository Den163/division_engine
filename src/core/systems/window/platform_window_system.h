#pragma once

#include <configs/engine_config.h>
#include <states/engine_state.h>

namespace PlatformWindowSystem
{
    void init(EngineState& engineState, const EngineConfig& engineConfig);
    void update(EngineState& engineState);
    void cleanup(EngineState& engineState);
}