#include "platform_window_system.h"
#include "../../configs/engine_config.h"
#include "../../utils/debug_utils.h"
#include "glfw_window_system.h"

#if defined(WIN32) || defined(_WIN32)
#include "win32_window_system.h"
#endif

void PlatformWindowSystem::init(EngineState& engineState, const EngineConfig& engineConfig)
{
#ifdef USE_GLFW
    GlfWindowSystem::init(engineState, engineConfig);
#endif

#if defined(WIN32) || defined(_WIN32)
    Win32WindowSystem::init(engineState);
#endif
}

void PlatformWindowSystem::update(EngineState& engineState)
{
#ifdef USE_GLFW
    GlfWindowSystem::update(engineState);
#endif
}

void PlatformWindowSystem::cleanup(EngineState& engineState)
{
#ifdef USE_GLFW
    GlfWindowSystem::cleanup(engineState);
#endif
}
