#include "window_system.h"

#define GLFW_INCLUDE_NONE
#include <chrono>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "../../utils/debug_utils.h"
#include "../components/glfw_window_data.h"
#include "../components/window_config.h"

void WindowSystem::init(singleton_registry& engineData)
{
    auto& windowState = engineData.emplace<WindowState>();
    windowState.lastUpdateTime = std::chrono::steady_clock::now();
    windowState.deltaTime = std::chrono::duration<float> { 0 };
    windowState.shouldClose = false;

    auto& glfwWindowData = engineData.emplace<GlfwWindowData>();
    auto& windowHandle = glfwWindowData.windowHandle;

    const auto& windowConfig = engineData.get<WindowConfig>();

    if (!glfwInit())
    {
        throw std::runtime_error {"Failed to init GLFW"};
    }

    glfwSetErrorCallback(DebugUtils::errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

void WindowSystem::update(singleton_registry& engineData)
{
    auto [ windowState, glfwWindowData, cfg ] = engineData.get<WindowState, const GlfwWindowData, const WindowConfig>();
    const auto now = std::chrono::steady_clock::now();
    const auto deltaTime = now - windowState.lastUpdateTime;

    windowState.shouldClose = glfwWindowShouldClose(glfwWindowData.windowHandle);
    windowState.deltaTime = deltaTime;
    windowState.lastUpdateTime = now;
}

void WindowSystem::cleanup(singleton_registry& engineData)
{
    auto& glfwWindowData = engineData.get<GlfwWindowData>();

    glfwDestroyWindow(glfwWindowData.windowHandle);
    glfwTerminate();
}

