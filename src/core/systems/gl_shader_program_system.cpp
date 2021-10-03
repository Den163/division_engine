#include "gl_shader_program_system.h"

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../utils/file_utils.h"
#include "../components/gl_shader_state.h"

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

static std::vector<ShaderInputInfo> collectShaderFileInfos();
static GLuint compileSingleShader(const ShaderInputInfo& shaderInfo);
static void throwLinkError(GLuint programHandle);

void GlShaderProgramSystem::init(GlShaderState& shaderProgram)
{
    auto& programHandle = shaderProgram.handle;

    programHandle = glCreateProgram();
    if (!programHandle) throw std::runtime_error {"Failed to create a program object"};

    auto shaderInfos = collectShaderFileInfos();
    std::vector<CompiledShaderInfo> compiledShaders { shaderInfos.size() };
    for (const auto& info: shaderInfos)
    {
        auto shaderHandle = compileSingleShader(info);
        compiledShaders.push_back(CompiledShaderInfo
            {
                shaderHandle,
                info.shaderType,
            });

        glAttachShader(programHandle, shaderHandle);
    }

    glLinkProgram(programHandle);

    GLint linkStatus;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);

    if (linkStatus)
    {
        glUseProgram(programHandle);
    }
    else
    {
        throwLinkError(programHandle);
    }
}

static std::vector<ShaderInputInfo> collectShaderFileInfos()
{
    std::vector<ShaderInputInfo> shaders;
    std::filesystem::directory_entry currentDir {std::filesystem::current_path().append("shaders")};
    std::filesystem::recursive_directory_iterator dirIter {currentDir};
    auto extensionToShaderTypeMap = SHADER_EXTENSION_TO_TYPE_MAP;

    for (const auto& entry: dirIter)
    {
        const auto& file = entry.path().filename();

        if (!entry.is_regular_file() ||
            file.extension() != SPIR_V_EXT_)
        {
            continue;
        }

        auto preExtension = file.stem().extension().string();
        if (extensionToShaderTypeMap.count(preExtension) == 0)
        {
            throw std::runtime_error {"Unknown shader extension: " + preExtension};
        }

        shaders.push_back(ShaderInputInfo
            {
                extensionToShaderTypeMap[preExtension],
                entry.path().string()
            });
    }

    return shaders;
}

static GLuint compileSingleShader(const ShaderInputInfo& shaderInfo)
{
    GLuint shader = glCreateShader(shaderInfo.shaderType);
    if (!shader) throw std::runtime_error {"Failed to create a shader!"};

    auto shaderBin = readBytes(shaderInfo.filePath);

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

void GlShaderProgramSystem::cleanup(GlShaderState& shaderProgram)
{
    glDeleteProgram(shaderProgram.handle);
}

