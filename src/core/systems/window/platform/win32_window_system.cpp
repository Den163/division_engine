#if defined(WIN32) || defined(_WIN32)

#include "win32_window_system.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

void Win32WindowSystem::init(EngineState& engineState)
{
    engineState.platform.win32.windowHandle = glfwGetWin32Window(engineState.platform.glfwWindow.windowHandle);
}

#endif