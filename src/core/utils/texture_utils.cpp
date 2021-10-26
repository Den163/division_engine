#include "texture_utils.h"

#include <stb_image.h>
#include <stdexcept>

static ColorMode getColorMode(int channels);
static GLenum getInternalFormat(ColorMode colorMode);

Texture2dState TextureUtils::loadFromFile(const std::string& filePath)
{
    int32_t channels;

    stbi_set_flip_vertically_on_load(true);

    int32_t width, height;
    uint8_t* imageData = stbi_load(filePath.data(), &width, &height, &channels, 0);

    if (imageData == nullptr)
    {
        throw std::runtime_error { "Failed to load texture: " + filePath };
    }

    auto colorMode = getColorMode(channels);
    auto textureState = loadFromBitmap(Texture2dConfig {
        .bitmapPtr = imageData,
        .width = width,
        .height = height,
        .colorMode = colorMode,
    });

    stbi_image_free(imageData);

    return textureState;
}

Texture2dState TextureUtils::loadFromBitmap(const Texture2dConfig& textureConfig)
{
    const auto colorMode = textureConfig.colorMode;
    Texture2dState textureState;

    glCreateTextures(GL_TEXTURE_2D, 1, &textureState.handle);
    glTextureStorage2D(textureState.handle, 1, getInternalFormat(colorMode), textureConfig.width, textureConfig.height);
    glTextureSubImage2D(
        textureState.handle, 0,
        0, 0,
        textureConfig.width, textureConfig.height,
        static_cast<GLenum>(colorMode), GL_UNSIGNED_BYTE, textureConfig.bitmapPtr);

    glTextureParameteri(textureState.handle, GL_TEXTURE_WRAP_S, textureConfig.coordinateFlags.s);
    glTextureParameteri(textureState.handle, GL_TEXTURE_WRAP_T, textureConfig.coordinateFlags.t);

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

GLenum getInternalFormat(ColorMode colorMode)
{
    switch (colorMode)
    {
        case ColorMode::RGB: return GL_RGB8;
        case ColorMode::RGBA: return GL_RGBA8;
        case ColorMode::R: return GL_R8;
        default: throw std::runtime_error {"Unrecognized color mode"};
    }
}
