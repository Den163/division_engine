#pragma once

#include "../configs/shader_pipeline_config.h"
#include "gl_shader_state.h"

#include <glad/gl.h>
#include <cstdint>

#include <vector>

struct GlShaderPipelineState
{
    GLuint glPipelineHandle;
};