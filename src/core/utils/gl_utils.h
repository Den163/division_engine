#pragma once

#include <cstdint>
#include <glad/gl.h>
#include <type_traits>
#include <stdexcept>

namespace GlUtils
{
    template<typename T1, typename T2>
    using gl_enum_when_same_types = typename std::enable_if<std::is_same<T1, T2>::value, GLenum>::type;

    template<typename T> constexpr gl_enum_when_same_types<T, float> mapToGlType() { return GL_FLOAT; }
    template<typename T> constexpr gl_enum_when_same_types<T, double> mapToGlType() { return GL_DOUBLE; }
    template<typename T> constexpr gl_enum_when_same_types<T, int8_t> mapToGlType() { return GL_BYTE; }
    template<typename T> constexpr gl_enum_when_same_types<T, uint8_t> mapToGlType() { return GL_UNSIGNED_BYTE; }
    template<typename T> constexpr gl_enum_when_same_types<T, uint16_t> mapToGlType() { return GL_UNSIGNED_SHORT; }
    template<typename T> constexpr gl_enum_when_same_types<T, int16_t> mapToGlType() { return GL_SHORT; }
    template<typename T> constexpr gl_enum_when_same_types<T, int32_t> mapToGlType() { return GL_INT; }
    template<typename T> constexpr gl_enum_when_same_types<T, int64_t> mapToGlType() { return GL_INT; }
    template<typename T> constexpr gl_enum_when_same_types<T, uint32_t> mapToGlType() { return GL_UNSIGNED_INT; }
    template<typename T> constexpr gl_enum_when_same_types<T, uint64_t> mapToGlType() { return GL_UNSIGNED_INT; }
}