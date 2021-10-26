#pragma once

#include <string>
#include "../primitives/gui/font.h"

namespace FontUtils
{
    void initializeFont(const std::string& fontFilePath, Font& font, const glm::ivec2& size);
}