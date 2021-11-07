#pragma once

#include <configs/engine_config.h>
#include <states/engine_state.h>

namespace RenderTickSystem
{
    void init(EngineState& engineState, const EngineConfig& engineConfig);
    void update(EngineState& engineState);
}