#include "win32_window_system.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

void Win32WindowSystem::init(EngineState& engineState)
{
    engineState.win32State.windowHandle = glfwGetWin32Window(engineState.glfwWindowState.windowHandle);
}
