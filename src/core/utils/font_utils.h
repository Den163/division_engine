#pragma once

#include <string>
#include "../primitives/gui/font.h"

namespace FontUtils
{
    Font makeFont(const std::string& fontFilePath, const glm::ivec2& size);
}