#pragma once

#include "glyph.h"

struct Font
{
    static constexpr uint8_t MAX_CHARACTER = 255;

    const Glyph& getGlyph(uint8_t character) const { return glyphs[character]; }

    Glyph glyphs[MAX_CHARACTER + 1];
};