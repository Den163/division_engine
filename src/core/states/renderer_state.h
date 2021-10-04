#pragma once

#include <chrono>

#include "update_time_state.h"

struct RendererState
{
    UpdateTimeState updateTimestamp;
    bool shouldUpdate;

    float fps() const { return 1 / updateTimestamp.deltaTime.count(); }
};
