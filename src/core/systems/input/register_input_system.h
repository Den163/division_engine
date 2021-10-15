#pragma once

#include "../../states/input_state.h"
#include "../../states/raw_input_state.h"
#include "../../states/renderer_state.h"

namespace RegisterInputSystem
{
void init(InputState& inputState);
void eventLoopUpdate(InputState& inputState, const RawInputState& rawInputState);
void postRenderUpdate(InputState& inputState, const RawInputState& rawInputState);
}