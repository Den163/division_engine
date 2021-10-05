#pragma once

#include <chrono>

#include "update_time_state.h"

struct RendererState
{
    UpdateTimeState updateTime;
    bool shouldUpdate;

    float fps() const { return 1 / updateTime.deltaTime.count(); }
};
