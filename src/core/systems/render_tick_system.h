#pragma once

#include "../components/renderer_state.h"
#include "../components/renderer_config.h"
#include "../components/window_state.h"

namespace RenderTickSystem
{
void init(RendererState& engineData);
void update(
    RendererState& rendererState, const RendererConfig& rendererConfig, const UpdateTimestamp& loopUpdateTimestamp);
}