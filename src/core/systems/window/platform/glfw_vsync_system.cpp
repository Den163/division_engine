#include "glfw_vsync_system.h"

void GlfwVsyncSystem::update(EngineState& engineState)
{
    glfwSwapBuffers(engineState.platform.glfwWindow.windowHandle);
}

