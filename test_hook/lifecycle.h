#pragma once

#include "../src/core/states/engine_state.h"
#include "global_state.h"

namespace Lifecycle
{
    void init(EngineState& state);

    void preRenderUpdate(EngineState& state);
    void postRenderUpdate(EngineState& state);

    void cleanup(EngineState& state);
}