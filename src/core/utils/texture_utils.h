#pragma once

#include "../states/texture2d_state.h"
#include "../configs/texture_2d_config.h"
#include <string>

namespace TextureUtils
{
    Texture2dState loadFromFile(const std::string& filePath);
    Texture2dState loadFromBitmap(const Texture2dConfig& textureConfig);
}