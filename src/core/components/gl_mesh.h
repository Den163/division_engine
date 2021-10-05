#pragma once

#include <glad/gl.h>

struct GlMesh
{
    static constexpr size_t VERTEX_POSITION_ATTRIB_INDEX = 0;
    static constexpr size_t COLOR_ATTRIB_INDEX = 1;
    static constexpr size_t VERTICES_IN_TRIANGLE = 3;

    GLuint vboHandle;
};