#pragma once

template<typename TState>
struct LifecycleConfig
{
    using EngineLifecycleFunc = std::function<void (TState& engineState)>;

    EngineLifecycleFunc init = [](auto& state){};
    EngineLifecycleFunc preRender = [](auto& state){};
    EngineLifecycleFunc postRender = [](auto& state){};
    EngineLifecycleFunc cleanup = [](auto& state){};
};