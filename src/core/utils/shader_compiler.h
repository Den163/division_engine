#pragma once

#include "../states/shader_state.h"
#include "../states/shader_pipeline_state.h"
#include "../configs/shader_config.h"

namespace ShaderCompiler
{
    ShaderState makeShader(const ShaderConfig& shader);
    ShaderPipelineState makePipeline();

    void deleteShader(const ShaderState& shader);
    void deletePipeline(const ShaderPipelineState& pipeline);
}