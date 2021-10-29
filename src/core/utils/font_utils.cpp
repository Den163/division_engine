#include "font_utils.h"

#include <glad/gl.h>
#include <stdexcept>
#include "texture_utils.h"
#include <ft2build.h>
#include FT_FREETYPE_H

static Glyph makeGlyph(FT_Face ftFace, Font::char_type character, const glm::vec2& size);
static Glyph makeEmptyGlyph(const glm::ivec2& size);

Font FontUtils::makeFont(const std::string& fontFilePath, const glm::ivec2& size)
{
    if (size.x == 0 && size.y == 0)
    {
        throw std::runtime_error{"At least one dimension in the size required to be greater than zero"};
    }

    FT_Library ft;
    auto initError = FT_Init_FreeType(&ft);
    if (initError)
    {
        throw std::runtime_error { "Failed to init FreeType library" };
    }

    FT_Face face;
    auto faceError = FT_New_Face(ft, fontFilePath.data(), 0, &face);
    if (faceError)
    {
        throw std::runtime_error { "Failed to load FreeType font face" };
    }

    FT_Set_Pixel_Sizes(face, size.x, size.y);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    Font font;
    auto emptinessSize = advanceToPixels(face->size->face->max_advance_width);
    auto emptyGlyph = makeEmptyGlyph(glm::vec2 {static_cast<float>(emptinessSize)});
    for (auto character = 0; character < Font::CHARACTERS_SIZE; character++)
    {
        auto& glyph = font.glyphs[character];

        glyph = makeGlyph(face, character, size);
        if (glyph.textureHandle == 0)
        {
            glyph = emptyGlyph;
        }
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return font;
}

Glyph makeGlyph(FT_Face ftFace, Font::char_type character, const glm::vec2& size)
{
    auto charError = FT_Load_Char(ftFace, character, FT_LOAD_RENDER);
    if (charError)
    {
        auto errorString = FT_Error_String(charError);
        throw std::runtime_error
        {
            "Failed to load FreeType character: " + std::string(errorString != nullptr ? errorString : "")
        };
    }

    const auto& glyph = ftFace->glyph;
    const auto& bitmap = glyph->bitmap;
    const auto width = bitmap.width;
    const auto height = bitmap.rows;

    if (width == 0 && height == 0)
    {
        return Glyph { .textureHandle = 0 };
    }

    auto texState = TextureUtils::loadFromBitmap(Texture2dConfig{
        .bitmapPtr = bitmap.buffer,
        .width = static_cast<int32_t>(width),
        .height = static_cast<int32_t>(height),
        .colorMode = ColorMode::Red,
        .coordinateFlags = Texture2dConfig::st_flags { Texture2dConfig::CLAMP_TO_EDGE },
    });

    return Glyph
    {
        .size = { width, height },
        .bearing = { glyph->bitmap_left, glyph->bitmap_top },
        .advance = static_cast<uint32_t>(glyph->advance.x),
        .textureHandle = texState.handle
    };
}

Glyph makeEmptyGlyph(const glm::ivec2& size)
{
    auto memSize = sizeof(uint8_t) * size.x * size.y;
    auto* emptyBitmapPtr = malloc(memSize);
    memset(emptyBitmapPtr, 0, memSize);
    auto texState = TextureUtils::loadFromBitmap(Texture2dConfig{
        .bitmapPtr = (uint8_t*) emptyBitmapPtr,
        .width = size.x,
        .height = size.y,
        .colorMode = ColorMode::Red,
        .coordinateFlags = Texture2dConfig::st_flags { Texture2dConfig::CLAMP_TO_EDGE }
    });

    free(emptyBitmapPtr);

    return Glyph
    {
        .size = size,
        .bearing = size,
        .advance = static_cast<uint32_t>(size.x) << 6,
        .textureHandle = texState.handle
    };
}
