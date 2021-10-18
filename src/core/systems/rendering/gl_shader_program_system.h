#pragma once

#include "../../states/gl_shader_state.h"
#include "../../configs/shader_config.h"
#include <vector>

namespace GlShaderProgramSystem
{
    void init(GlShaderState& shaderState, const std::vector<ShaderConfig>& shaderConfigs);
    void cleanup(GlShaderState& shaderProgram);
}