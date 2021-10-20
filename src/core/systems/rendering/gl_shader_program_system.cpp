#include "gl_shader_program_system.h"

#include <fstream>
#include <string>
#include <vector>

#include "../../../utils/file_utils.h"

static constexpr const char* SHADERS_DIR_ = "shaders/";
static constexpr const char* SPIR_V_EXT_ = ".spv";

static std::vector<GlShaderPipelineState> makeShaderPipelines(
    const ShaderPipelineConfig& pipelineConfigs,
    const std::vector<ShaderConfig>& shaderConfigs,
    const std::vector<GlShaderState>& shaderStates);

static std::vector<GlShaderState> makeShaders(const std::vector<ShaderConfig>& shaderConfigs);
static GLuint compileSingleShader(const ShaderConfig& config);
static void checkProgramStatus(GLuint programHandle);
static std::string getProgramInfoLog(GLuint programHandle);

void GlShaderProgramSystem::init(EngineState& engineState, const EngineConfig& engineConfig)
{
    if (engineConfig.shaders.empty())
    {
        throw std::runtime_error { "Can't run engine, because shaders or shader pipelines are not configured" };
    }

    engineState.shaderStates = makeShaders(engineConfig.shaders);
    engineState.shaderPipelineStates = makeShaderPipelines(
        engineConfig.shaderPipeline, engineConfig.shaders, engineState.shaderStates);
}

std::vector<GlShaderState> makeShaders(const std::vector<ShaderConfig>& shaderConfigs)
{
    auto shaderStates = std::vector<GlShaderState> { shaderConfigs.size() };

    for (size_t i = 0; i < shaderConfigs.size(); i++)
    {
        auto& shaderState = shaderStates[i];
        const auto& config = shaderConfigs[i];

        auto programHandle = glCreateProgram();
        glProgramParameteri(programHandle, GL_PROGRAM_SEPARABLE, GL_TRUE);

        auto shaderHandle = compileSingleShader(config);
        glAttachShader(programHandle, shaderHandle);
        glLinkProgram(programHandle);
        checkProgramStatus(programHandle);

        shaderState.glProgramHandle = programHandle;
        shaderState.glShaderHandle = shaderHandle;
    }

    return shaderStates;
}

static GLuint compileSingleShader(const ShaderConfig& config)
{
    GLuint shader = glCreateShader(static_cast<GLenum>(config.type));
    if (!shader) throw std::runtime_error {"Failed to makeDefault a shader!"};

    auto shaderBin = readBytes(SHADERS_DIR_ + config.name + SPIR_V_EXT_);

    glShaderBinary(
        1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, shaderBin.data(), static_cast<GLsizei>(shaderBin.size()));
    GLint binaryLoadResult = 0;
    glGetShaderiv(shader, GL_SPIR_V_BINARY, &binaryLoadResult);
    if (binaryLoadResult == GL_FALSE) throw std::runtime_error {"Failed to load SPIR-V shader binary!"};

    glSpecializeShaderARB(shader, "main", 0, nullptr, nullptr);
    GLint compilationResult = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationResult);
    if (compilationResult == GL_TRUE) return shader;

    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    std::string infoLog(maxLength, ' ');
    glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());
    infoLog.resize(maxLength);

    throw std::runtime_error
        {
            std::string {"Failed to specialize shader SPIR-V binary. Info log: \n"} + infoLog + "\n"
        };
}

void checkProgramStatus(GLuint programHandle)
{
    GLint linkStatus;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
    {
        const auto error = getProgramInfoLog(programHandle);
        throw std::runtime_error {"Failed to link a shader program. Info log: \n" + error};
    }

    glValidateProgram(programHandle);
    GLint validateStatus;
    glGetProgramiv(programHandle, GL_VALIDATE_STATUS, &validateStatus);
    if (validateStatus == GL_FALSE)
    {
        const auto error = getProgramInfoLog(programHandle);
        throw std::runtime_error{"Failed to validate a shader program. Info log: \n" + error};
    }
}

static std::string getProgramInfoLog(GLuint programHandle)
{
    GLint linkErrorLength;
    glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &linkErrorLength);
    std::string error((size_t) linkErrorLength, ' ');
    glGetProgramInfoLog(programHandle, linkErrorLength, &linkErrorLength, error.data());

    return error;
}

std::vector<GlShaderPipelineState> makeShaderPipelines(
    const ShaderPipelineConfig& pipelineConfig,
    const std::vector<ShaderConfig>& shaderConfigs,
    const std::vector<GlShaderState>& shaderStates)
{
    const auto pipelinesCount = pipelineConfig.pipelinesCount;
    auto pipelineStates = std::vector<GlShaderPipelineState> {pipelinesCount};

    for (size_t pipelineIdx = 0; pipelineIdx < pipelinesCount; pipelineIdx++)
    {
        auto& pipelineState = pipelineStates[pipelineIdx];
        glCreateProgramPipelines(1, &pipelineState.glPipelineHandle);
    }

    return pipelineStates;
}

void GlShaderProgramSystem::cleanup(EngineState& engineState)
{
    for (const auto& pipelineState: engineState.shaderPipelineStates)
    {
        glDeleteProgramPipelines(1, &pipelineState.glPipelineHandle);
    }

    for (const auto& shaderState: engineState.shaderStates)
    {
        glDeleteProgram(shaderState.glProgramHandle);
    }
}