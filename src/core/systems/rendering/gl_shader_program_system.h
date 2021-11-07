#pragma once

#include <configs/engine_config.h>
#include <states/engine_state.h>

namespace GlShaderProgramSystem
{
    void init(EngineState& engineState, const EngineConfig& engineConfig);
    void cleanup(EngineState& engineState);
}