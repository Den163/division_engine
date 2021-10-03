#pragma once

#include <chrono>

#include "update_timestamp.h"

struct RendererState
{
    UpdateTimestamp updateTimestamp;
    bool shouldUpdate;

    float fps() const { return 1 / updateTimestamp.deltaTime.count(); }
};
