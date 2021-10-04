#include "glfw_window_system.h"

#include <stdexcept>
#include <glad/gl.h>

#include "../../utils/debug_utils.h"

void GlfWindowSystem::init(WindowState& windowState, GlfwWindowConfig& glfwWindowData, const WindowConfig& windowConfig)
{
    auto& windowHandle = glfwWindowData.windowHandle;
    windowState.shouldClose = false;

    if (!glfwInit())
    {
        throw std::runtime_error {"Failed to preInit GLFW"};
    }

    glfwSetErrorCallback(DebugUtils::windowErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    windowHandle = glfwCreateWindow(
        windowConfig.width, windowConfig.height, windowConfig.title.data(), nullptr, nullptr);

    if (!windowHandle)
    {
        throw std::runtime_error {"Failed to create a window_"};
    }

    glfwMakeContextCurrent(windowHandle);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        throw std::runtime_error {"Failed to call gladLoadGL"};
    }
}

void GlfWindowSystem::update(WindowState& windowState, GlfwWindowConfig& glfwWindowData)
{
    windowState.shouldClose = glfwWindowShouldClose(glfwWindowData.windowHandle);
}

void GlfWindowSystem::cleanup(GlfwWindowConfig& glfwWindowData)
{
    glfwDestroyWindow(glfwWindowData.windowHandle);
    glfwTerminate();
}

