#include "platform_window_system.h"

#if defined(WIN32) || defined(_WIN32)
#include "win32_window_system.h"
#endif

void PlatformWindowSystem::init(EngineState& engineState)
{
#if defined(WIN32) || defined(_WIN32)
    Win32WindowSystem::init(engineState);
#endif
}
