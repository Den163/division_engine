#pragma once

#include "../../states/gl_shader_state.h"

namespace GlShaderProgramSystem
{
    void init(GlShaderState& shaderState);
    void cleanup(GlShaderState& shaderProgram);
}