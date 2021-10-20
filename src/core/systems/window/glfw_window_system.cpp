#include "glfw_window_system.h"

#include <stdexcept>

#include "../../../utils/debug_utils.h"

void GlfWindowSystem::init(EngineState& engineState, const EngineConfig& engineConfig)
{
    auto& windowState = engineState.windowState;
    auto& windowHandle = engineState.glfwWindowState.windowHandle;
    const auto& windowConfig = engineConfig.window;

    windowState.width = windowConfig.width;
    windowState.height = windowConfig.height;
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


    windowHandle = glfwCreateWindow(windowState.width, windowState.height, windowConfig.title.data(), nullptr, nullptr);
    if (!windowHandle)
    {
        throw std::runtime_error {"Failed to makeDefault a window_"};
    }

    glfwMakeContextCurrent(windowHandle);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        throw std::runtime_error {"Failed to call gladLoadGL"};
    }
}

void GlfWindowSystem::update(EngineState& engineState)
{
    auto& windowState = engineState.windowState;
    auto& windowHandle = engineState.glfwWindowState.windowHandle;

    glfwPollEvents();
    windowState.shouldClose = glfwWindowShouldClose(windowHandle);
    glfwGetWindowSize(windowHandle, &windowState.width, &windowState.height);
}

void GlfWindowSystem::cleanup(EngineState& engineState)
{
    glfwDestroyWindow(engineState.glfwWindowState.windowHandle);
    glfwTerminate();
}

