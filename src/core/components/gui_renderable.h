#pragma once

#include <glad/gl.h>

struct GuiRenderable
{
    GLuint shaderPipelineHandle;
    GLuint vertexShaderProgramHandle;
    GLuint fragmentShaderProgramHandle;
};