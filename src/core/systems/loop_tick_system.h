#pragma once

#include "../components/window_state.h"

namespace LoopTickSystem
{
void init(UpdateTimestamp& loopUpdateTimestamp);
void update(UpdateTimestamp& loopUpdateTimestamp);
}