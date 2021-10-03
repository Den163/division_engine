#pragma once

#include "../components/gl_shader_state.h"

namespace GlShaderProgramSystem
{
void init(GlShaderState& shaderProgram);

void cleanup(GlShaderState& shaderProgram);
}