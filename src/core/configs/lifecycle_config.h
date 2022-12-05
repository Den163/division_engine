#pragma once

#include <states/engine_state.h>

struct LifecycleConfig
{
    using EngineLifecycleFunc = std::function<void (EngineState& state)>;

    EngineLifecycleFunc init = [](EngineState&){};
    EngineLifecycleFunc preRender = [](EngineState&){};
    EngineLifecycleFunc postRender = [](EngineState&){};
    EngineLifecycleFunc cleanup = [](EngineState&){};
};