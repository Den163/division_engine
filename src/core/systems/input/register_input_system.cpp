#include <iostream>
#include "register_input_system.h"

#include "../../../utils/math.h"

void RegisterInputSystem::init(InputState& inputState)
{
    for (auto& key : inputState.keyboardState.keyFlags)
    {
        key = InputState::KEY_STATE_NONE;
    }
}

void RegisterInputSystem::eventLoopUpdate(InputState& inputState, const RawInputState& rawInputState)
{
    const auto& rawKeyboardState = rawInputState.keyboardState;
    auto& keyboardState = inputState.keyboardState;

    for (size_t i = 0; i < KeyboardState::KEYS_COUNT; i++)
    {
        const auto& rawKeyState = rawKeyboardState.keyFlags[i];
        auto& keyState = keyboardState.keyFlags[i];

        auto rawKeyPressed = hasFlag(rawKeyState, RawInputState::KEY_STATE_PRESSED);
        auto keyWasPressed = hasFlag(keyState, InputState::KEY_STATE_PRESSED);
        auto keyWasReleased = hasFlag(keyState, InputState::KEY_STATE_RELEASED);
        auto keyWasInSomeState = !hasFlag(keyState, InputState::KEY_STATE_NONE);

        auto keyWillPressed = rawKeyPressed && !keyWasInSomeState;
        auto keyWillHold = rawKeyPressed && (keyWillPressed || keyWasPressed || keyWasReleased);
        auto keyWillReleased = !rawKeyPressed && keyWasInSomeState;
        auto keyWillReset = !rawKeyPressed && !keyWasInSomeState;

        if (keyWillPressed)
        {
            keyState = InputState::KEY_STATE_PRESSED;
        }
        else if (keyWillHold)
        {
            keyState |= InputState::KEY_STATE_HOLD;
        }
        else if (keyWillReleased)
        {
            keyState |= InputState::KEY_STATE_RELEASED;
        }
        else if (keyWillReset)
        {
            keyState = InputState::KEY_STATE_NONE;
        }
    }
}

void RegisterInputSystem::postRenderUpdate(InputState& inputState)
{
    auto& keyboardState = inputState.keyboardState;

    for (size_t i = 0; i < KeyboardState::KEYS_COUNT; i++)
    {
        auto& keyState = keyboardState.keyFlags[i];

        if (hasFlag(keyState, InputState::KEY_STATE_PRESSED))
        {
            keyState = (keyState ^ InputState::KEY_STATE_PRESSED);
        }

        if (hasFlag(keyState, InputState::KEY_STATE_RELEASED))
        {
            keyState = InputState::KEY_STATE_NONE;
        }
    }
}
