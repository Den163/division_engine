#pragma once

#include <chrono>

struct RendererState
{
    using float_seconds = std::chrono::duration<float>;
    using steady_clock = std::chrono::steady_clock;
    using steady_clock_time_point = std::chrono::time_point<steady_clock>;

    steady_clock_time_point lastUpdateTime;
    float_seconds deltaTime;
    bool shouldUpdate;

    float fps() const { return 1 / deltaTime.count(); }
};
