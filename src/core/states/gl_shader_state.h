#pragma once

#include <glad/gl.h>
#include <cstdint>

#include <vector>

struct GlShaderState
{
    GLint programHandle;
    GLuint vaoHandle;
};