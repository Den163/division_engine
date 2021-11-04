#pragma once

#include <glad/gl.h>

struct GuiMaterial
{
    GLuint shaderPipelineHandle;
    GLuint vertexShaderProgramHandle;
    GLuint fragmentShaderProgramHandle;
};