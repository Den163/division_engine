#include "gl_shader_program_system.h"

#include <utils/debug_utils.h>
#include <utils/shader_compiler.h>

void GlShaderProgramSystem::init(EngineState& engineState, const EngineConfig& engineConfig)
{
    glEnable(GL_DEBUG_OUTPUT);
    glClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE);
    glDebugMessageCallback(DebugUtils::glRendererMessageCallback, nullptr);

    for (size_t i = 0; i < DefaultShadersConfig::defaultShaders.size(); i++)
    {
        const auto& shaderConfig = DefaultShadersConfig::defaultShaders[i];
        engineState.defaultShader.shaders[i] = ShaderCompiler::makeShader(shaderConfig);
    }
    engineState.defaultShader.pipeline = ShaderCompiler::makePipeline();
}

void GlShaderProgramSystem::cleanup(EngineState& engineState)
{
    ShaderCompiler::deletePipeline(engineState.defaultShader.pipeline);
    for (const auto& shader : engineState.defaultShader.shaders)
    {
        ShaderCompiler::deleteShader(shader);
    }
}