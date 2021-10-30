#include "texture_utils.h"

#include <stb_image.h>
#include <stb_image_write.h>
#include <stdexcept>

static ColorMode getColorMode(int channels);
static GLenum getInternalFormat(ColorMode colorMode);
static int getComp(ColorMode colorMode);

Texture2dState TextureUtils::loadFromFile(const std::string& filePath)
{
    int32_t channels;

    stbi_set_flip_vertically_on_load(true);

    glm::ivec2 size;
    uint8_t* imageData = stbi_load(filePath.data(), &size.x, &size.y, &channels, 0);

    if (imageData == nullptr)
    {
        throw std::runtime_error { "Failed to load texture: " + filePath };
    }

    auto colorMode = getColorMode(channels);
    auto textureState = loadFromBitmap(Texture2dConfig {
        .size = size,
        .offset = glm::ivec2{ 0 },
        .colorMode = colorMode,
        .bitmapPtr = imageData,
    });

    stbi_image_free(imageData);

    return textureState;
}

Texture2dState TextureUtils::loadFromBitmap(const Texture2dConfig& textureConfig)
{
    auto textureState = reserveTextureBuffer(textureConfig);
    loadToBuffer(textureState, textureConfig);

    return textureState;
}

Texture2dState TextureUtils::reserveTextureBuffer(const Texture2dConfig& textureConfig)
{
    const auto& size = textureConfig.size;

    Texture2dState textureState;
    glCreateTextures(GL_TEXTURE_2D, 1, &textureState.handle);
    glTextureStorage2D(textureState.handle, 1, getInternalFormat(textureConfig.colorMode), size.x, size.y);
    glTextureParameteri(textureState.handle, GL_TEXTURE_WRAP_S, textureConfig.coordinateFlags.s);
    glTextureParameteri(textureState.handle, GL_TEXTURE_WRAP_T, textureConfig.coordinateFlags.t);

    return textureState;
}

void TextureUtils::loadToBuffer(Texture2dState& textureState, const Texture2dConfig& textureConfig)
{
    const auto& offset = textureConfig.offset;
    const auto& size = textureConfig.size;

    glTextureSubImage2D(
        textureState.handle, 0,
        offset.x, offset.y,
        size.x, size.y,
        static_cast<GLenum>(textureConfig.colorMode), GL_UNSIGNED_BYTE, textureConfig.bitmapPtr);
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
        case ColorMode::Red: return GL_R8;
        default: throw std::runtime_error {"Unrecognized color mode"};
    }
}

void TextureUtils::writeToFile(
    const std::string& filePath, uint8_t* buffer, const glm::ivec2& size, ColorMode colorMode)
{
    stbi_write_png(filePath.data(), size.x, size.y, getComp(colorMode), buffer, size.x);
}

int getComp(ColorMode colorMode)
{
    switch (colorMode)
    {
        case ColorMode::Red: return 1;
        case ColorMode::RGB: return 3;
        case ColorMode::RGBA: return 4;
        default: throw std::runtime_error {"Unrecognized color mode"};
    }
}
