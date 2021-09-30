#pragma once

#include <chrono>

struct RendererState
{
    std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;
    bool shouldUpdate;
};