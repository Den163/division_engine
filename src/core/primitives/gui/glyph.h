#pragma once

#include <cstdint>
#include <glm/vec2.hpp>

struct Glyph
{
    glm::ivec2 size;
    glm::ivec2 bearing;
    uint8_t advance;
    uint8_t textureHandle;
};