#pragma once

#include "../states/engine_state.h"
#include "engine_invariants.h"

namespace EngineStateHelper
{
    inline ShaderState& standardVertexShaderProgram(EngineState& engineState)
    {
        return engineState.defaultShader.shaders[EngineInvariants::STANDARD_VERTEX_SHADER_INDEX];
    }

    inline ShaderState& standardColorFragmentShaderProgram(EngineState& engineState)
    {
        return engineState.defaultShader.shaders[EngineInvariants::STANDARD_COLORED_FRAGMENT_SHADER_INDEX];
    }

    inline ShaderState& standardTextureFragmentShaderProgram(EngineState& engineState)
    {
        return engineState.defaultShader.shaders[EngineInvariants::STANDARD_TEXTURE_FRAGMENT_SHADER_INDEX];
    }

    inline ShaderPipelineState& standardShaderPipeline(EngineState& engineState)
    {
        return engineState.defaultShader.pipeline;
    }

    inline Texture2dState& texture2d(EngineState& engineState, uint32_t textureIndex)
    {
        return engineState.textures[textureIndex];
    }
}