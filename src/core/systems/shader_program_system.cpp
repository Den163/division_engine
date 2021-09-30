#include "shader_program_system.h"

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../utils/file_utils.h"
#include "../components/gl_shader_program.h"

static const std::vector<float> positions {
    -0.8f, -0.8f, 0.0f,
    0.8f, -0.8f, 0.0f,
    0.0f, 0.8f, 0.0f
};

static const std::vector<float> colors {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

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
static void drawTestData(singleton_registry& singletonRegistry);

void ShaderProgramSystem::init(singleton_registry& singletonRegistry)
{
    auto& programComponent = singletonRegistry.emplace<GLShaderProgram>();
    auto& programHandle = programComponent.shaderProgramHandle;

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
        drawTestData(singletonRegistry);
        return;
    }

    GLint linkErrorLength;
    glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &linkErrorLength);
    std::string error(static_cast<size_t>(linkErrorLength), ' ');
    glGetProgramInfoLog(programHandle, linkErrorLength, &linkErrorLength, error.data());
    error.resize(linkErrorLength);

    throw std::runtime_error {"Failed to link a shader program. Info log: \n" + error};
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

    std::vector<uint8_t> shaderBin = readBytes(shaderInfo.filePath);
    glShaderBinary(
        1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderBin.data(), static_cast<GLsizei>(shaderBin.size()));
    GLint binaryLoadResult = 0;
    glGetShaderiv(shader, GL_SPIR_V_BINARY, &binaryLoadResult);
    if (!binaryLoadResult) throw std::runtime_error {"Failed to load SPIR-V shader binary!"};

    glSpecializeShaderARB(shader, "main", 0, nullptr, nullptr);
    GLint compilationResult = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationResult);
    if (compilationResult) return shader;

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

static void drawTestData(singleton_registry& singletonRegistry)
{
    auto& programComponent = singletonRegistry.get<GLShaderProgram>();
    auto& vaoHandle = programComponent.vertexArrayObject;

    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(positions.size()), positions.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(colors.size()), colors.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(vaoHandle);
}

void ShaderProgramSystem::cleanup(singleton_registry& singletonRegistry)
{
    const auto& shaderProgram = singletonRegistry.get<GLShaderProgram>();
    glDeleteProgram(shaderProgram.shaderProgramHandle);

    singletonRegistry.erase<GLShaderProgram>();
}

