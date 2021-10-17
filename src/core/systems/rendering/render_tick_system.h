#pragma once

#include "../../states/renderer_state.h"
#include "../../configs/renderer_config.h"
#include "../../states/window_state.h"

namespace RenderTickSystem
{
    void init(RendererState& rendererState, const RendererConfig& rendererConfig);
    void update(RendererState& rendererState, const UpdateTimeState& loopUpdateTimestamp);
}