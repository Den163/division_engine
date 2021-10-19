#pragma once

#include <glad/gl.h>

struct GlMesh
{
    static constexpr size_t COLOR_ATTRIB_INDEX = 0;
    static constexpr size_t VERTEX_ATTRIB_INDEX = 1;
    static constexpr size_t MVP_MATRIX_UNIFORM_INDEX = 2;

    GLuint vertexVboHandle;
};