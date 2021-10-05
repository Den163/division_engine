#include "glfw_vsync_system.h"

#include <iostream>

#include "../configs/glfw_window_config.h"

void GlfwVsyncSystem::update(GlfwWindowConfig& glfwWindowData)
{
    glfwSwapBuffers(glfwWindowData.windowHandle);
}

