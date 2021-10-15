#pragma once

#include "../../states/input_state.h"
#include "../../states/raw_input_state.h"
#include "../../states/win32_state.h"

namespace Win32RegisterInputSystem
{
void init(RawInputState& rawInputState);
void update(RawInputState& rawInputState, const Win32State& win32State);
}