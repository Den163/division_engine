#pragma once

#include "../../states/engine_state.h"

namespace DebugRenderPassSystem
{
    void init(EngineState& engineState);
    void preRender(EngineState& engineState);
    void postRender(EngineState& engineState);
}