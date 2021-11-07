#pragma once

#include <primitives/gui/glyph.h>

struct Font
{
    using char_type = uint8_t;
    static constexpr char_type MAX_CHARACTER = 255;
    static constexpr size_t CHARACTERS_SIZE = MAX_CHARACTER + 1;

    const Glyph& getGlyph(char_type character) const { return glyphs[character]; }

    Glyph glyphs[CHARACTERS_SIZE];
    glm::ivec2 textureSize;
    uint32_t fontHeight;
    uint32_t textureHandle;
};