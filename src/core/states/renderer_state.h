#pragma once

#include <chrono>
#include <glm/vec4.hpp>

#include <states/update_time_state.h>

struct RendererState
{
    float targetFps;
    bool shouldUpdate;
    UpdateDeltaTimeState frameDelta;
    UpdateDeltaTimeState renderPass;

    glm::vec4 backgroundColor;

    inline float fps() const { return 1 / frameDelta.deltaTime.count(); }
};
