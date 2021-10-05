#pragma once

#include "keyboard_state.h"

#include <unordered_set>

struct RawInputState
{
    static const uint8_t KEY_STATE_NONE = 1;
    static const uint8_t KEY_STATE_PRESSED = 1 << 1;

    KeyboardState keyboardState;
};