#include "platform_register_input_system.h"

#if defined(WIN32) || defined(_WIN32)
#include "win32_register_input_system.h"
#endif

void PlatformRegisterInputSystem::init(EngineState& engineState)
{
#if defined(WIN32) || defined(_WIN32)
    Win32RegisterInputSystem::init(engineState);
#endif
}

void PlatformRegisterInputSystem::update(EngineState& engineState)
{
#if defined(WIN32) || defined(_WIN32)
    Win32RegisterInputSystem::update(engineState);
#endif
}
