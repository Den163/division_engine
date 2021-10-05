#pragma once

#include <glad/gl.h>
#include <cstdint>

#include <vector>

struct GlShaderState
{
    static constexpr size_t VERTEX_ARRAYS_COUNT = 1;
    static constexpr size_t TRIANGLES_ARRAY_INDEX = 0;

    GLint programHandle;
    GLuint vertexArrayHandles[VERTEX_ARRAYS_COUNT];
};