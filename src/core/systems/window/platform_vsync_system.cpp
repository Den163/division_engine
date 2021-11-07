#include "platform_vsync_system.h"

#include "glfw_vsync_system.h"

void PlatformVSyncSystem::update(EngineState& engineState)
{
#ifdef USE_GLFW
    GlfwVsyncSystem::update(engineState);
#endif
}
