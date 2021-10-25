#include "shader_compiler.h"
#include "file_utils.h"

static constexpr const char* SPIR_V_EXT_ = ".spv";

static void checkProgramStatus(GLuint programHandle);
static std::string getProgramInfoLog(GLuint programHandle);
static GLuint readFromSpirv(const ShaderConfig& config);

ShaderState ShaderCompiler::makeShader(const ShaderConfig& shaderConfig)
{
    auto programHandle = glCreateProgram();
    glProgramParameteri(programHandle, GL_PROGRAM_SEPARABLE, GL_TRUE);

    auto shaderHandle = readFromSpirv(shaderConfig);
    glAttachShader(programHandle, shaderHandle);
    glLinkProgram(programHandle);
    checkProgramStatus(programHandle);

    ShaderState shaderState;
    shaderState.programHandle = programHandle;
    shaderState.shaderHandle = shaderHandle;

    return shaderState;
}

ShaderPipelineState ShaderCompiler::makePipeline()
{
    ShaderPipelineState pipelineState;
    glCreateProgramPipelines(1, &pipelineState.handle);
    return pipelineState;
}

void ShaderCompiler::deleteShader(const ShaderState& shader)
{
    glDeleteShader(shader.shaderHandle);
    glDeleteProgram(shader.programHandle);
}

void ShaderCompiler::deletePipeline(const ShaderPipelineState& pipeline)
{
    glDeleteProgramPipelines(1, reinterpret_cast<const GLuint*>(&pipeline.handle));
}

GLuint readFromSpirv(const ShaderConfig& config)
{
    GLuint shader = glCreateShader(static_cast<GLenum>(config.type));
    if (!shader) throw std::runtime_error {"Failed to makeDefault a shader!"};

    auto shaderBin = readBytes(std::string(config.name) + SPIR_V_EXT_);

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