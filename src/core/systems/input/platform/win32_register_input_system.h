#pragma once

#if defined(WIN32) || defined(_WIN32)

#include "../../states/engine_state.h"

namespace Win32RegisterInputSystem
{
    void init(EngineState& engineState);
    void update(EngineState& engineState);
}

#endif