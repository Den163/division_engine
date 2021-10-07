#include "win32_register_input_system.h"

#include <iostream>
#include <stdexcept>
#include <Windows.h>

#include "../../../utils/math.h"

constexpr auto BITS_IN_BYTES = 8;
constexpr uint8_t WIN32_KEY_PRESSED_MASK = 1 << (sizeof(BYTE) * BITS_IN_BYTES - 1);

void Win32RegisterInputSystem::init(RawInputState& rawInputState)
{
    for (auto& keyFlag : rawInputState.keyboardState.keyFlags)
    {
        keyFlag = RawInputState::KEY_STATE_NONE;
    }
}

void Win32RegisterInputSystem::update(RawInputState& rawInputState)
{
    BYTE winKeyboardState[KeyboardState::KEYS_COUNT];
    if (!GetKeyboardState(winKeyboardState))
    {
        throw std::runtime_error {"Cannot get the keyboard state (via Win32API)"};
    }

    for (size_t i = 0; i < KeyboardState::KEYS_COUNT; i++)
    {
        auto winKeyPressed = hasFlag(winKeyboardState[i], WIN32_KEY_PRESSED_MASK);
        auto keyState = selectScalar(winKeyPressed, RawInputState::KEY_STATE_PRESSED, RawInputState::KEY_STATE_NONE);
        rawInputState.keyboardState.keyFlags[i] = keyState;
    }
}

