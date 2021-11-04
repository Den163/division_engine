#pragma once

#include <glad/gl.h>

struct GlMesh
{
    GLuint vaoHandle;
    GLuint vertexVboHandle;
    GLuint modelViewProjectionVboHandle;
    GLuint indirectBufferHandle;
};