#include "glfw_vsync_system.h"

#include <iostream>

#include "../components/glfw_window_data.h"

void GlfwVSyncSystem::update(GlfwWindowData& glfwWindowData)
{
    glfwSwapBuffers(glfwWindowData.windowHandle);
}

