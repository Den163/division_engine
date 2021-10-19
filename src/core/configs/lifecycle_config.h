#pragma once

typedef std::function<void (EngineState& engineState)> EngineLifecycleFunc;

struct LifecycleConfig
{
    EngineLifecycleFunc init = [](auto& state){};
    EngineLifecycleFunc preRender = [](auto& state){};
    EngineLifecycleFunc postRender = [](auto& state){};
    EngineLifecycleFunc cleanup = [](auto& state){};
};