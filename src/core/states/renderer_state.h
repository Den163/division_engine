#pragma once

#include <chrono>

#include "update_time_state.h"
#include <glm/vec4.hpp>

struct RendererState
{
    float targetFps;
    bool shouldUpdate;
    UpdateDeltaTimeState frameDelta;
    UpdateDeltaTimeState renderPass;

    glm::vec4 backgroundColor;

    inline float fps() const { return 1 / frameDelta.deltaTime.count(); }
};
