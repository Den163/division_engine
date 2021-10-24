#pragma once

#include "../core/states/engine_state.h"
#include "engine_invariants.h"

namespace EngineStateHelper
{
    inline GLuint& shaderProgram(EngineState& engineState, uint32_t shaderIndex)
    {
        return engineState.shaders[shaderIndex].glProgramHandle;
    }

    inline GLuint& standardVertexShaderProgram(EngineState& engineState)
    {
        return shaderProgram(engineState, EngineInvariants::STANDARD_VERTEX_SHADER_INDEX);
    }

    inline GLuint& standardFragmentShaderProgram(EngineState& engineState)
    {
        return shaderProgram(engineState, EngineInvariants::STANDARD_FRAGMENT_SHADER_INDEX);
    }

    inline GLuint& shaderPipeline(EngineState& engineState, uint32_t pipelineIndex)
    {
        return engineState.shaderPipelines[pipelineIndex].glPipelineHandle;
    }

    inline GLuint& standardShaderPipeline(EngineState& engineState)
    {
        return shaderPipeline(engineState, EngineInvariants::STANDARD_SHADER_PIPELINE_INDEX);
    }
}