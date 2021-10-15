#pragma once

#include "keyboard_state.h"

#include <cstdint>
#include <glm/vec2.hpp>
#include <unordered_set>

struct InputState
{
    static const uint8_t KEY_STATE_NONE = 1;
    static const uint8_t KEY_STATE_PRESSED = 1 << 1;
    static const uint8_t KEY_STATE_HOLD = 1 << 2;
    static const uint8_t KEY_STATE_RELEASED = 1 << 3;

    KeysState keyboardState;
    glm::vec2 postRenderMouseDelta;
    glm::vec2 postRenderMousePosition;

    template<typename TKey>
    bool keyPressed(TKey key) const { return keyboardState.keyHasFlag(key, KEY_STATE_PRESSED); }
    template<typename TKey>
    bool keyHold(TKey key) const { return keyboardState.keyHasFlag(key, KEY_STATE_HOLD); }
};