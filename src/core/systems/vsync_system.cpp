#include "vsync_system.h"

#include <iostream>

#include "../components/glfw_window_data.h"

void VSyncSystem::init(singleton_registry& engineData)
{

}

void VSyncSystem::update(singleton_registry& engineData)
{
    const auto& glfwWindowData = engineData.get<GlfwWindowData>();
    glfwSwapBuffers(glfwWindowData.windowHandle);
}

void VSyncSystem::cleanup(singleton_registry& engineData)
{

}
