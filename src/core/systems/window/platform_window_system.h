#pragma once

#include "../../states/engine_state.h"
#include "../../configs/engine_config.h"

namespace PlatformWindowSystem
{
    void init(EngineState& engineState, const EngineConfig& engineConfig);
    void update(EngineState& engineState);
    void cleanup(EngineState& engineState);
}