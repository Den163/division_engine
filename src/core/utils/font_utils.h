#pragma once

#include <string>
#include "../primitives/gui/font.h"

namespace FontUtils
{
    Font makeFont(const std::string& fontFilePath, uint32_t fontHeight);
    inline int32_t advanceToPixels(int32_t advance) { return advance >> 6; }
    inline int32_t pixelsToAdvance(int32_t pixels) { return pixels << 6; }
}