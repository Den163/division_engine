#pragma once

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct GlfwWindowState
{
    GLFWwindow* windowHandle;
};