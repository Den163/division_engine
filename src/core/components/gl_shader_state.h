#pragma once

#include <glad/gl.h>
#include <cstdint>

#include <vector>

struct GlShaderState
{
    static constexpr size_t VERTEX_ARRAYS_COUNT = 1;
    static constexpr size_t TRIANGLES_ARRAY_INDEX = 0;

    static constexpr size_t VERTEX_BUFFERS_COUNT = 2;
    static constexpr size_t POSITIONS_VBO_INDEX = 0;
    static constexpr size_t COLORS_VBO_INDEX = 1;

    GLint programHandle;
    GLuint vertexArrayHandles[VERTEX_ARRAYS_COUNT];
    GLuint vertexBufferHandles[VERTEX_BUFFERS_COUNT];

    std::vector<float> vertexBuffer { 0 };
};