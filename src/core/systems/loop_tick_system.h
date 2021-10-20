#pragma once

#include "../states/window_state.h"
#include "../states/update_time_state.h"
#include "../states/engine_state.h"

namespace LoopTickSystem
{
    void init(EngineState& engineState);
    void update(EngineState& engineState);
}