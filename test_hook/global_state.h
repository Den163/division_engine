#pragma once

#include "../src/core/states/engine_state.h"
#include "../src/core/configs/engine_config.h"
#include "../src/core/configs/lifecycle_config.h"

struct GlobalState
{
    EngineState engineState;
    EngineConfig engineConfig;
    LifecycleConfig<GlobalState> lifecycle;
};