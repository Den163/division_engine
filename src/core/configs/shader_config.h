#pragma once

#include <stdexcept>
#include <glad/gl.h>
#include <string>

enum class ShaderType
{
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
};

struct ShaderConfig
{
    const char* name;
    ShaderType type;

    GLbitfield getTypeBit() const
    {
        switch(type)
        {
            case ShaderType::Vertex : return GL_VERTEX_SHADER_BIT;
            case ShaderType::Fragment: return GL_FRAGMENT_SHADER_BIT;
        }

        throw std::runtime_error { "Invalid type" };
    }
};