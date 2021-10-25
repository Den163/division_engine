#include "register_input_system.h"

#include "../../utils/math.h"
#include "math.h"

void RegisterInputSystem::init(EngineState& engineState)
{
    auto& inputState = engineState.input;

    for (auto& key : inputState.keyboardState.keyFlags)
    {
        key = InputState::KEY_STATE_NONE;
    }

    inputState.postRenderMouseDelta = glm::vec2 {0};
    inputState.postRenderMousePosition = glm::vec2 {0};
}

void RegisterInputSystem::eventLoop(EngineState& engineState)
{
    auto& inputState = engineState.input;
    auto& keyboardState = inputState.keyboardState;

    const auto& rawInputState = engineState.rawInput;
    const auto& rawKeyboardState = rawInputState.keyboardState;

    for (size_t i = 0; i < KeysState::KEYS_COUNT; i++)
    {
        const auto& rawKeyState = rawKeyboardState.keyFlags[i];
        auto& keyState = keyboardState.keyFlags[i];

        auto rawKeyPressed = Math::hasFlag(rawKeyState, RawInputState::KEY_STATE_PRESSED);
        auto keyWasPressed = Math::hasFlag(keyState, InputState::KEY_STATE_PRESSED);
        auto keyWasReleased = Math::hasFlag(keyState, InputState::KEY_STATE_RELEASED);
        auto keyWasInSomeState = !Math::hasFlag(keyState, InputState::KEY_STATE_NONE);

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

void RegisterInputSystem::postRender(EngineState& engineState)
{
    auto& inputState = engineState.input;
    const auto& rawInputState = engineState.rawInput;

    for (size_t i = 0; i < KeysState::KEYS_COUNT; i++)
    {
        auto& keyState = inputState.keyboardState.keyFlags[i];

        if (Math::hasFlag(keyState, InputState::KEY_STATE_PRESSED))
        {
            keyState = (keyState ^ InputState::KEY_STATE_PRESSED);
        }

        if (Math::hasFlag(keyState, InputState::KEY_STATE_RELEASED))
        {
            keyState = InputState::KEY_STATE_NONE;
        }
    }

    inputState.postRenderMouseDelta = rawInputState.eventLoopMousePosition - inputState.postRenderMousePosition;
    inputState.postRenderMousePosition = rawInputState.eventLoopMousePosition;
}
