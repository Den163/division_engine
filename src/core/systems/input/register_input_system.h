#pragma once

#include "../../states/input_state.h"
#include "../../states/raw_input_state.h"
#include "../../states/renderer_state.h"

namespace RegisterInputSystem
{
    void init(InputState& inputState);
    void eventLoop(InputState& inputState, const RawInputState& rawInputState);
    void postRender(InputState& inputState, const RawInputState& rawInputState);
}