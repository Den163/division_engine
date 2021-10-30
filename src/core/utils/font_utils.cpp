#include "font_utils.h"

#include <format>
#include <glad/gl.h>
#include <stdexcept>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "texture_utils.h"

static inline void loadFontAtlas(FT_Face ftFace, Font& font, const glm::ivec2& glyphSize);
static inline void throwFreetypeException(const char* formattedExceptionText, int freetypeErrorCode);

Font FontUtils::makeFont(const std::string& fontFilePath, const glm::ivec2& size)
{
    if (size.x == 0 && size.y == 0)
    {
        throw std::runtime_error{"At least one dimension in the size required to be greater than zero"};
    }

    FT_Library ft;
    auto initError = FT_Init_FreeType(&ft);
    if (initError) throwFreetypeException("Failed to init FreeType library: {}", initError);

    FT_Face face;
    auto faceError = FT_New_Face(ft, fontFilePath.data(), 0, &face);
    if (faceError) throwFreetypeException("Failed to load Freetype font face: {}", faceError);

    FT_Set_Pixel_Sizes(face, size.x, size.y);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    Font font;
    loadFontAtlas(face, font, size.x == 0 ? glm::ivec2{size.y} : glm::ivec2{size.x});

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return font;
}

void loadFontAtlas(FT_Face ftFace, Font& font, const glm::ivec2& glyphSize)
{
    const auto reservedCharacters = 256;
    const auto maxAdvance = ftFace->max_advance_width;
    const auto glyphPixels = glyphSize.x * glyphSize.y;
    const int bitmapRows = sqrt(reservedCharacters);
    const int bitmapColumns = bitmapRows;
    const auto bitmapPixelWidth = glyphSize.x * bitmapRows;
    const auto bitmapPixelHeight = glyphSize.y * bitmapColumns;
    auto* bitmap = (uint8_t*) std::malloc(glyphPixels * reservedCharacters);

    for (size_t character = 0; character < reservedCharacters; character++)
    {
        auto charErr = FT_Load_Char(ftFace, character, FT_LOAD_RENDER);
        if (charErr) throwFreetypeException("Can't load character: {}", charErr);

        const auto bmpCol = character % bitmapColumns;
        const auto bmpRow = character / bitmapRows;
        const auto* ftGlyph = ftFace->glyph;
        const auto& ftBitmap = ftGlyph->bitmap;

        font.glyphs[character] = Glyph
        {
            .size = { ftBitmap.width, ftBitmap.rows },
            .offset = { bmpCol * glyphSize.x, bmpRow * glyphSize.y },
            .bearing = { ftGlyph->bitmap_left, ftGlyph->bitmap_top },
            .advance = static_cast<uint32_t>(ftGlyph->advance.x),
        };

        for (auto row = 0; row < glyphSize.x; row++)
        {
            for (auto column = 0; column < glyphSize.y; column++)
            {
                const auto x = bmpCol * glyphSize.x + column;
                const auto y = bmpRow * glyphSize.y + row;
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
    throw std::runtime_error{ std::format(std::string {formattedExceptionText}, err) };
}
