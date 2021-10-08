#include "glfw_vsync_system.h"

#include <iostream>

#include "../states/glfw_window_state.h"

void GlfwVsyncSystem::update(GlfwWindowState& glfwWindowData)
{
    glfwSwapBuffers(glfwWindowData.windowHandle);
}

