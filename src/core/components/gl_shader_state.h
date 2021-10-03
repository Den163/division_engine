#pragma once

#include <glad/gl.h>
#include <cstdint>

struct GlShaderState
{
    GLint handle;
    GLuint vaoHandle;
    GLuint vboHandles[2];

    static constexpr uint32_t VBO_COUNT = 2;
    static constexpr uint32_t POSITIONS_VBO_INDEX = 0;
    static constexpr uint32_t COLORS_VBO_INDEX = 1;
};