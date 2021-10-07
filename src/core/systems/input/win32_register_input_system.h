#pragma once

#include "../../states/input_state.h"
#include "../../states/raw_input_state.h"

namespace Win32RegisterInputSystem
{
void init(RawInputState& rawInputState);
void update(RawInputState& rawInputState);
}