#pragma once

#include "../core/states/engine_state.h"
#include "engine_invariants.h"

namespace EngineStateHelper
{
    inline GLuint& standardVertexShaderProgram(EngineState& engineState)
    {
        return engineState.shaderStates[EngineInvariants::STANDARD_VERTEX_SHADER_INDEX].glProgramHandle;
    }

    inline GLuint& standardFragmentShaderProgram(EngineState& engineState)
    {
        return engineState.shaderStates[EngineInvariants::STANDARD_FRAGMENT_SHADER_INDEX].glProgramHandle;
    }

    inline GLuint& standardShaderPipeline(EngineState& engineState)
    {
        return engineState.shaderPipelineStates[EngineInvariants::STANDARD_SHADER_PIPELINE_INDEX].glPipelineHandle;
    }
}