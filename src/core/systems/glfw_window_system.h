#pragma once

#include "../states/window_state.h"
#include "../configs/glfw_window_config.h"
#include "../configs/window_config.h"

namespace GlfWindowSystem
{
void init(WindowState& windowState, GlfwWindowConfig& glfwWindowData, const WindowConfig& windowConfig);
void update(WindowState& windowState, GlfwWindowConfig& glfwWindowData);
void cleanup(GlfwWindowConfig& glfwWindowData);
}