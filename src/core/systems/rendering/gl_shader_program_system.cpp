#include "gl_shader_program_system.h"

#include <fstream>
#include <string>
#include <vector>

#include "../../../utils/file_utils.h"

static constexpr const char* SHADERS_DIR_ = "shaders/";
static constexpr const char* SPIR_V_EXT_ = ".spv";

#define SHADER_EXTENSION_TO_TYPE_MAP        \
    std::unordered_map<std::string, GLenum> \
    {                                       \
        {".vert", GL_VERTEX_SHADER},        \
        {".frag", GL_FRAGMENT_SHADER},      \
    }

struct ShaderInputInfo
{
    GLenum shaderType;
    std::string filePath;
};

struct CompiledShaderInfo
{
    GLuint handle;
    GLenum shaderType;
};

static GLuint compileSingleShader(const ShaderConfig& config);
static void throwLinkError(GLuint programHandle);

void GlShaderProgramSystem::init(EngineState& engineState, const EngineConfig& engineConfig)
{
    auto& shaderState = engineState.shaderState;

    const auto& shaderConfigs = engineConfig.shaders;
    const auto configsSize = shaderConfigs.size();
    if (configsSize == 0) return;

    shaderState.programHandle = glCreateProgram();
    glCreateVertexArrays(1, &shaderState.vaoHandle);

    const auto programHandle = shaderState.programHandle;
    if (!programHandle) throw std::runtime_error {"Failed to create a program object"};

    std::vector<CompiledShaderInfo> compiledShaders { configsSize };
    for (size_t i = 0; i < configsSize; i++)
    {
        const auto& config = shaderConfigs[i];
        auto shaderHandle = compileSingleShader(config);
        compiledShaders[i] = CompiledShaderInfo { shaderHandle, static_cast<GLenum>(config.type) };

        glAttachShader(programHandle, shaderHandle);
    }

    glLinkProgram(programHandle);

    GLint linkStatus;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_TRUE)
    {
        glUseProgram(programHandle);
    }
    else
    {
        throwLinkError(programHandle);
    }
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

static void throwLinkError(GLuint programHandle)
{
    GLint linkErrorLength;
    glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &linkErrorLength);
    std::string error(static_cast<size_t>(linkErrorLength), ' ');
    glGetProgramInfoLog(programHandle, linkErrorLength, &linkErrorLength, error.data());
    error.resize(linkErrorLength);

    throw std::runtime_error {"Failed to link a shader program. Info log: \n" + error};
}

void GlShaderProgramSystem::cleanup(EngineState& engineState)
{
    glDeleteProgram(engineState.shaderState.programHandle);
}

