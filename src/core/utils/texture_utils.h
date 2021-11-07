#pragma once

#include <string>

#include <configs/texture_2d_config.h>
#include <states/texture2d_state.h>

namespace TextureUtils
{
    Texture2dState loadFromFile(const std::string& filePath);
    Texture2dState loadFromBitmap(const Texture2dConfig& textureConfig);
    Texture2dState reserveTextureBuffer(const Texture2dConfig& textureConfig);
    void loadToBuffer(Texture2dState& textureState, const Texture2dConfig& textureConfig);
    void writeToFile(const std::string& filePath, uint8_t* buffer, const glm::ivec2& size, ColorMode colorMode);
}