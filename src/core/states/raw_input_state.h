#pragma once

#include <glm/vec2.hpp>
#include <unordered_set>

#include <states/keyboard_state.h>

struct RawInputState
{
    static const uint8_t KEY_STATE_NONE = 1;
    static const uint8_t KEY_STATE_PRESSED = 1 << 1;

    KeysState keyboardState;
    glm::vec2 eventLoopMousePosition;
};