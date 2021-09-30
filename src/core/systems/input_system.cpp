#include "input_system.h"

#include <GLFW/glfw3.h>
#include <iostream>

void InputSystem::init()
{
}

void InputSystem::update()
{
    glfwPollEvents();
}

void InputSystem::cleanup()
{
}
