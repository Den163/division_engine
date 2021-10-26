#include "font_utils.h"

#include <glad/gl.h>
#include <stdexcept>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "texture_utils.h"

void FontUtils::initializeFont(const std::string& fontFilePath, Font& font, const glm::ivec2& size)
{
    if (size.x == 0 && size.y == 0)
    {
        throw std::runtime_error { "At least one dimension in the division_engine_core required to be greater than zero" };
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

    for (uint8_t character = 0; character <= Font::MAX_CHARACTER; character++)
    {
        auto charError = FT_Load_Char(face, character, FT_LOAD_RENDER);
        if (charError)
        {
            throw std::runtime_error { "Failed to load FreeType character" };
        }

        auto& glyph = font.glyphs[character];
        const auto& bitmap = face->glyph->bitmap;

        auto texState = TextureUtils::loadFromBitmap(Texture2dConfig{
            .bitmapPtr = bitmap.buffer,
            .width = static_cast<int32_t>(bitmap.width),
            .height = static_cast<int32_t>(bitmap.rows),
            .coordinateFlags = { Texture2dConfig::CLAMP_TO_EDGE, Texture2dConfig::CLAMP_TO_EDGE },
        });

        glyph.textureHandle = texState.handle;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
