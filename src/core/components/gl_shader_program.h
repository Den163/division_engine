#pragma once

#include <glad/gl.h>

struct GLShaderProgram
{
    GLint handle;
    GLuint vaoHandle;
    GLuint vboHandles[2];

    static constexpr int32_t VBO_COUNT = 2;
    static constexpr int32_t POSITIONS_VBO_INDEX = 0;
    static constexpr int32_t COLORS_VBO_INDEX = 1;
};