#pragma once

#include <cstdint>
#include <glm/vec2.hpp>

struct Glyph
{
    glm::ivec2 size;
    glm::ivec2 textureOffset;
    glm::ivec2 bearing;
    uint32_t advance;
};