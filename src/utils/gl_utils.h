#pragma once

#include <cstdint>
#include <glad/gl.h>
#include <type_traits>

namespace GlUtils
{
    template<typename T>
    inline constexpr int32_t mapToGlType()
    {
        constexpr auto isFloat = std::is_same<T, float>();
        constexpr auto isInt = std::is_same<T, float>();
        static_assert(
            isFloat || isInt,
            "Can't map to GL type"
        );

        if constexpr(isFloat)
        {
            return GL_FLOAT;
        }
        else if constexpr(isInt)
        {
            return GL_INT;
        }
    }
}