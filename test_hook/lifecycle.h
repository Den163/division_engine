#pragma once

#include "../src/core/states/engine_state.h"
#include "global_state.h"

namespace Lifecycle
{
    void init(GlobalState& state);

    void preRenderUpdate(GlobalState& state);
    void postRenderUpdate(GlobalState& state);

    void cleanup(GlobalState& state);
}