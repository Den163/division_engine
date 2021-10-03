#include "glfw_input_system.h"

#include <GLFW/glfw3.h>

void GlfwInputSystem::update()
{
    glfwPollEvents();
}

