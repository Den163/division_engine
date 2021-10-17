#pragma once

#include "configs/renderer_config.h"
#include "configs/window_config.h"
#include <type_traits>

namespace EngineCore
{
    void run(WindowConfig& windowConfig, RendererConfig& rendererConfig);
}