#pragma once

#include <states/engine_state.h>

namespace RegisterInputSystem
{
    void init(EngineState& engineState);
    void eventLoop(EngineState& engineState);
    void postRender(EngineState& engineState);
}