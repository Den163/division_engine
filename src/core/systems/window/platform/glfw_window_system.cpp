#include "glfw_window_system.h"

#include <stdexcept>

#include "../../../utils/debug_utils.h"

void GlfWindowSystem::init(EngineState& engineState, const EngineConfig& engineConfig)
{
    auto& windowState = engineState.window;
    auto& windowHandle = engineState.platform.glfwWindow.windowHandle;
    const auto& windowConfig = engineConfig.window;

    windowState.screenSpaceWidth = windowConfig.width;
    windowState.screenSpaceHeight = windowConfig.height;
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
        windowState.screenSpaceWidth, windowState.screenSpaceHeight, windowConfig.title.data(), nullptr, nullptr);
    if (!windowHandle)
    {
        throw std::runtime_error {"Failed to makeDefault a window_"};
    }

    glfwMakeContextCurrent(windowHandle);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        throw std::runtime_error {"Failed to call gladLoadGL"};
    }

    glfwGetFramebufferSize(windowHandle, &windowState.frameBufferPixelsWidth, &windowState.frameBufferPixelsHeight);
}

void GlfWindowSystem::update(EngineState& engineState)
{
    auto& windowState = engineState.window;
    auto& windowHandle = engineState.platform.glfwWindow.windowHandle;

    glfwPollEvents();
    windowState.shouldClose = glfwWindowShouldClose(windowHandle);

    glfwGetFramebufferSize(windowHandle, &windowState.frameBufferPixelsWidth, &windowState.frameBufferPixelsHeight);
    glfwGetWindowSize(windowHandle, &windowState.screenSpaceWidth, &windowState.screenSpaceHeight);
    glViewport(0, 0, windowState.frameBufferPixelsWidth, windowState.frameBufferPixelsHeight);
}

void GlfWindowSystem::cleanup(EngineState& engineState)
{
    glfwDestroyWindow(engineState.platform.glfwWindow.windowHandle);
    glfwTerminate();
}

