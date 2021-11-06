#include "font_utils.h"

#include <glad/gl.h>
#include <stdexcept>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "texture_utils.h"

static inline void loadFontAtlas(FT_Face ftFace, Font& font, uint32_t fontHeight);
static inline void throwFreetypeException(const char* formattedExceptionText, int freetypeErrorCode);

Font FontUtils::makeFont(const std::string& fontFilePath, uint32_t fontHeight)
{
    if (fontHeight == 0)
    {
        throw std::runtime_error{"At least one dimension in the size required to be greater than zero"};
    }

    FT_Library ft;
    auto initError = FT_Init_FreeType(&ft);
    if (initError) throwFreetypeException("Failed to init FreeType library: ", initError);

    FT_Face face;
    auto faceError = FT_New_Face(ft, fontFilePath.data(), 0, &face);
    if (faceError) throwFreetypeException("Failed to load Freetype font face: ", faceError);

    FT_Set_Pixel_Sizes(face, 0, fontHeight);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    Font font;
    font.fontHeight = fontHeight;
    loadFontAtlas(face, font, fontHeight);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return font;
}

void loadFontAtlas(FT_Face ftFace, Font& font, uint32_t fontHeight)
{
    const auto reservedCharacters = 256;
    const auto maxAdvance = ftFace->max_advance_width;
    const auto glyphPixels = fontHeight * fontHeight;
    const int bitmapRows = sqrt(reservedCharacters);
    const int bitmapColumns = bitmapRows;
    const auto bitmapPixelWidth = fontHeight * bitmapRows;
    const auto bitmapPixelHeight = fontHeight * bitmapColumns;
    auto* bitmap = (uint8_t*) std::malloc(glyphPixels * reservedCharacters);

    for (size_t character = 0; character < reservedCharacters; character++)
    {
        auto charErr = FT_Load_Char(ftFace, character, FT_LOAD_RENDER);
        if (charErr) throwFreetypeException("Can't load character: ", charErr);

        const auto bmpCol = character % bitmapColumns;
        const auto bmpRow = character / bitmapRows;
        const auto* ftGlyph = ftFace->glyph;
        const auto& ftBitmap = ftGlyph->bitmap;

        font.glyphs[character] = Glyph
        {
            .size = { ftBitmap.width, ftBitmap.rows },
            .textureOffset = {bmpCol * fontHeight, bmpRow * fontHeight },
            .bearing = { ftGlyph->bitmap_left, ftGlyph->bitmap_top },
            .advance = static_cast<uint32_t>(ftGlyph->advance.x),
        };

        for (auto row = 0; row < fontHeight; row++)
        {
            for (auto column = 0; column < fontHeight; column++)
            {
                const auto x = bmpCol * fontHeight + column;
                const auto y = bmpRow * fontHeight + row;
                const auto i = x + bitmapPixelWidth * y;

                if (row >= ftBitmap.rows || column >= ftBitmap.width)
                {
                    bitmap[i] = 0;
                }
                else
                {
                    bitmap[i] = ftBitmap.buffer[column + ftBitmap.width * row];
                }
            }
        }
    }

    const auto& textureSize = glm::ivec2 { bitmapPixelWidth, bitmapPixelHeight };
    auto texState = TextureUtils::loadFromBitmap(Texture2dConfig{
       .size = textureSize,
       .offset = glm::ivec2 { 0 },
       .coordinateFlags = Texture2dConfig::st_flags { Texture2dConfig::CLAMP_TO_EDGE },
       .colorMode = ColorMode::Red,
       .bitmapPtr = bitmap
    });

    std::free(bitmap);

    font.textureSize = textureSize;
    font.textureHandle = texState.handle;
}

void throwFreetypeException(const char* formattedExceptionText, int freetypeErrorCode)
{
    auto err = FT_Error_String(freetypeErrorCode);
    if (err == nullptr) err = "";
    throw std::runtime_error{ formattedExceptionText + std::string{err} };
}
