#pragma once

#include "../components/window_state.h"
#include "../components/glfw_window_data.h"
#include "../components/window_config.h"

namespace GlfWindowSystem
{
void init(WindowState& windowState, GlfwWindowData& glfwWindowData, const WindowConfig& windowConfig);
void update(WindowState& windowState, GlfwWindowData& glfwWindowData);
void cleanup(GlfwWindowData& glfwWindowData);
}