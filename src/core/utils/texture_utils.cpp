#include "texture_utils.h"

#include <stb_image.h>
#include <stdexcept>

static ColorMode getColorMode(int channels);

Texture2dState TextureUtils::loadTextureFromFile(const std::string& filePath)
{
    Texture2dState textureState;

    int32_t channels;

    stbi_set_flip_vertically_on_load(true);
    uint8_t* imageData = stbi_load(filePath.data(), &textureState.width, &textureState.height, &channels, 0);

    if (imageData == nullptr)
    {
        throw std::runtime_error { "Failed to load texture: " + filePath };
    }

    textureState.colorMode = getColorMode(channels);
    glCreateTextures(GL_TEXTURE_2D, 1, &textureState.handle);
    glTextureStorage2D(textureState.handle, 1, GL_RGB8, textureState.width, textureState.height);
    glTextureSubImage2D(
        textureState.handle, 0,
        0, 0,
        textureState.width, textureState.height,
        GL_RGB, GL_UNSIGNED_BYTE, imageData);

    stbi_image_free(imageData);

    return textureState;
}

ColorMode getColorMode(int channels)
{
    switch (channels)
    {
        case 3: return ColorMode::RGB;
        case 4: return ColorMode::RGBA;
        default:
            throw std::runtime_error { "Can't map channels count: `" + std::to_string(channels) + "` to ColorMode" };
    }
}