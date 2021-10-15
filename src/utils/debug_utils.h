#pragma once

#include <glad/gl.h>
#include <glm/vec3.hpp>
#include <iostream>
#include <string>

namespace DebugUtils
{
void windowErrorCallback(int error, const char* description);
void printRendererInfo();
void GLAPIENTRY glRendererMessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);

template<glm::length_t Length, typename Type, glm::qualifier Q>
std::string toString(const glm::vec<Length, Type, Q>& vec)
{
    std::string str { "{ " };
    for (glm::length_t i = 0; i < Length - 1; i++)
    {
        str.append(std::to_string(vec[i]).append(", "));
    }

    str.append(std::to_string(vec[Length - 1]).append(" }"));
    return str;
}
}

template<class Traits, glm::length_t Length, typename Type, glm::qualifier Q>
std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& ostream,
    const glm::vec<Length, Type, Q> vec)
{
    return ostream << DebugUtils::template toString(vec);
}