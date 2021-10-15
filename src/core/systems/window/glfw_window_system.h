#pragma once

#include "../../states/window_state.h"
#include "../../states/glfw_window_state.h"
#include "../../configs/window_config.h"

namespace GlfWindowSystem
{
void init(WindowState& windowState, GlfwWindowState& glfwWindowData, const WindowConfig& windowConfig);
void update(WindowState& windowState, GlfwWindowState& glfwWindowData);
void cleanup(GlfwWindowState& glfwWindowData);
}