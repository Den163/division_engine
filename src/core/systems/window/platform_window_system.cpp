#include "platform_window_system.h"

#include <systems/window/platform/glfw_window_system.h>
#include <systems/window/platform/win32_window_system.h>

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
