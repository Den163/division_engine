#pragma once

#include "keyboard_state.h"

#include <cstdint>
#include <unordered_set>

struct InputState
{
    static const uint8_t KEY_STATE_NONE = 1;
    static const uint8_t KEY_STATE_PRESSED = 1 << 1;
    static const uint8_t KEY_STATE_HOLD = 1 << 2;
    static const uint8_t KEY_STATE_RELEASED = 1 << 3;

    KeyboardState keyboardState;
};