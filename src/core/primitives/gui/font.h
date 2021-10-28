#pragma once

#include "glyph.h"
#include "gui_quad.h"

struct Font
{
    static constexpr uint8_t MAX_CHARACTER = 255;
    static constexpr size_t CHARACTERS_SIZE = MAX_CHARACTER + 1;

    const Glyph& getGlyph(uint8_t character) const { return glyphs[character]; }

    Glyph glyphs[CHARACTERS_SIZE];
};