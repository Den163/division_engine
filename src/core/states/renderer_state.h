#pragma once

#include <chrono>

#include "update_time_state.h"
#include <glm/vec4.hpp>

struct RendererState
{
    float targetFps;
    bool shouldUpdate;
    UpdateTimeState updateTime;

    glm::vec4 backgroundColor;

    inline float fps() const { return 1 / updateTime.deltaTime.count(); }
};
