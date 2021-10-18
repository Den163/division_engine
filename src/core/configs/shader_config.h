#pragma once

#include <glad/gl.h>
#include <string>

enum class ShaderType
{
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
};

struct ShaderConfig
{
    std::string name;
    ShaderType type;
};