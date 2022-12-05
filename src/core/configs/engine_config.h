#pragma once

#include <functional>
#include <vector>

#include <configs/renderer_config.h>
#include <configs/window_config.h>
#include <configs/lifecycle_config.h>

struct EngineConfig
{
    RendererConfig renderer;
    WindowConfig window;
    LifecycleConfig lifeCycle;
};