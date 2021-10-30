#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include "color_mode.h"

struct Texture2dConfig
{
    enum TextureCoordinateFlags
    {
        NONE = 0x0,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
        REPEAT = GL_REPEAT,
        DEFAULT = REPEAT
    };

    using st_flags = glm::vec<2, TextureCoordinateFlags>;

    glm::ivec2 size;
    glm::ivec2 offset;
    st_flags coordinateFlags = { DEFAULT, DEFAULT };
    ColorMode colorMode;
    uint8_t* bitmapPtr;
};