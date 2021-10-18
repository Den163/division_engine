#pragma once

typedef std::function<void (EngineState& engineState)> EngineLifecycleFunc;

struct LifecycleConfig
{
    EngineLifecycleFunc init = [](auto& state){};
    EngineLifecycleFunc preRenderUpdate = [](auto& state){};
    EngineLifecycleFunc postRenderUpdate = [](auto& state){};
    EngineLifecycleFunc cleanup = [](auto& state){};
};