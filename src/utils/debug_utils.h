#pragma once

#include <iostream>
#include <glad/gl.h>

namespace DebugUtils
{
void windowErrorCallback(int error, const char* description);
void printRendererInfo();

void GLAPIENTRY glRendererMessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);
}
