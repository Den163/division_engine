#pragma once

#include "../core/engine_state.h"

namespace Lifecycle
{
void preInit(EngineState& state);
void postInit(EngineState& state);
void beginLoopUpdate(EngineState& state);
void preRenderUpdate(EngineState& state);
void postRenderUpdate(EngineState& state);
void endLoopUpdate(EngineState& state);
void preCleanup(EngineState& state);
void postCleanup(EngineState& state);
}