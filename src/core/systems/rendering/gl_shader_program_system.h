#pragma once

#include "../../states/engine_state.h"
#include "../../configs/engine_config.h"
#include <vector>

namespace GlShaderProgramSystem
{
    void init(EngineState& engineState, const EngineConfig& engineConfig);
    void cleanup(EngineState& engineState);
}