#pragma once

#include "../../states/engine_state.h"
#include "../../configs/engine_config.h"

namespace RenderTickSystem
{
    void init(EngineState& engineState, const EngineConfig& engineConfig);
    void update(EngineState& engineState);
}