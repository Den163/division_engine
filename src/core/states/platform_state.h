#pragma once

#include <states/platform/win32_state.h>

struct PlatformState
{
#if defined(WIN32) || defined(_WIN32)
    Win32State win32;
#endif
#if defined(USE_GLFW)
    GlfwWindowState glfwWindow;
#endif
};
