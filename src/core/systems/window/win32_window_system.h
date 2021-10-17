#pragma once

#include "../../states/win32_state.h"
#include "../../states/glfw_window_state.h"

namespace Win32WindowSystem
{
    void init(Win32State& win32State, const GlfwWindowState& glfwWindowState);
};