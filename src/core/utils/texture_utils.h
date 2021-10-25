#pragma once

#include "../states/texture2d_state.h"
#include <string>

namespace TextureUtils
{
    Texture2dState loadTextureFromFile(const std::string& filePath);
}