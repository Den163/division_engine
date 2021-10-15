#include "win32_register_input_system.h"

#include <iostream>
#include <stdexcept>
#include <Windows.h>

#include "../../../utils/math.h"
#include "math.h"

constexpr auto BITS_IN_BYTES = 8;
constexpr uint8_t WIN32_KEY_PRESSED_MASK = 1 << (sizeof(BYTE) * BITS_IN_BYTES - 1);

void Win32RegisterInputSystem::init(RawInputState& rawInputState)
{
    for (auto& keyFlag : rawInputState.keyboardState.keyFlags)
    {
        keyFlag = RawInputState::KEY_STATE_NONE;
    }

    rawInputState.eventLoopMousePosition = glm::vec2 {0};
}

void Win32RegisterInputSystem::update(RawInputState& rawInputState, const Win32State& win32State)
{
    BYTE winKeyboardState[KeysState::KEYS_COUNT];
    if (!GetKeyboardState(winKeyboardState))
    {
        throw std::runtime_error {"Cannot get the keyboard state (via Win32API)"};
    }

    for (size_t i = 0; i < KeysState::KEYS_COUNT; i++)
    {
        auto winKeyPressed = Math::hasFlag(winKeyboardState[i], WIN32_KEY_PRESSED_MASK);
        auto keyState = Math::selectScalar(winKeyPressed, RawInputState::KEY_STATE_PRESSED, RawInputState::KEY_STATE_NONE);
        rawInputState.keyboardState.keyFlags[i] = keyState;
    }

    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(win32State.windowHandle, &cursorPos);
    rawInputState.eventLoopMousePosition = glm::vec2 { cursorPos.x, cursorPos.y };
}

