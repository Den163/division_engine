#pragma once

#include "../states/window_state.h"
#include "../states/update_time_state.h"

namespace LoopTickSystem
{
    void init(UpdateTimeState& loopUpdateTimestamp);
    void update(UpdateTimeState& loopUpdateTimestamp);
}