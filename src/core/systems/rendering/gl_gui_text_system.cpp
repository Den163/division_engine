#include "gl_gui_text_system.h"

#include "../../components/gl_draw_indirect_command.h"
#include "../../components/gl_mesh.h"
#include "../../components/gui_material.h"
#include "../../components/gui_text.h"
#include "../../utils/font_utils.h"
#include "../../utils/color.h"
#include "../../components/gl_texture.h"
#include "../../utils/gl_utils.h"

constexpr size_t VERTICES_FOR_CHARACTER_ = 4;

static inline void applyIndirectBuffer(const GlMesh& glMesh, size_t charactersCount);
static inline void fillTextBuffer(const std::string& text, const Font& font, GuiVertex* vertexData);

void GlGuiTextSystem::update(EngineState& engineState)
{
    auto& registry = engineState.guiRegistry;
    const auto& windowState = engineState.window;

    for (auto&&[e, glMesh, glTexture, material, guiText] :
         registry.view<GlMesh, GlTexture, const GuiMaterial, const GuiText>().each())
    {
        const auto text = guiText.text;
        const auto charactersCount = text.size();
        const auto verticesCount = charactersCount * VERTICES_FOR_CHARACTER_;
        const auto newBufferSize = verticesCount * sizeof(GuiVertex);
        const auto vboHandle = glMesh.vertexVboHandle;
        const auto fontIndex = guiText.font;
        const auto textSize = glm::vec2 { 0, guiText.fontHeight };

        glBindVertexArray(glMesh.vaoHandle);

        int32_t bufferSize = 0;
        glGetNamedBufferParameteriv(vboHandle, GL_BUFFER_SIZE, &bufferSize);

        if (bufferSize == 0)
        {
            glNamedBufferStorage(
                vboHandle, newBufferSize, nullptr, GL_MAP_WRITE_BIT);
            glNamedBufferStorage(
                glMesh.indirectBufferHandle, sizeof(GlDrawArraysIndirectCommand) * charactersCount, nullptr,
                GL_MAP_WRITE_BIT);
        }

        const auto& font = engineState.resources.fonts.get(fontIndex);
        glTexture.handle = font.textureHandle;

        GuiVertex* vertexData = (GuiVertex*) glMapNamedBuffer(vboHandle, GL_WRITE_ONLY);
        fillTextBuffer(text, font, vertexData);
        glUnmapNamedBuffer(vboHandle);

        applyIndirectBuffer(glMesh, charactersCount);
        glMesh.verticesCount = verticesCount;
        glMesh.primitivesCount = charactersCount;
    }
}

void applyIndirectBuffer(const GlMesh& glMesh, size_t charactersCount)
{
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, glMesh.indirectBufferHandle);
    auto* indirectsData = (GlDrawArraysIndirectCommand*) glMapNamedBuffer(glMesh.indirectBufferHandle, GL_WRITE_ONLY);
    for (size_t i = 0; i < charactersCount; i++)
    {
        auto& indirect = indirectsData[i];
        indirect.first = i * VERTICES_FOR_CHARACTER_;
        indirect.count = VERTICES_FOR_CHARACTER_;
        indirect.baseInstanceOffset = 0;
        indirect.primitivesCount = charactersCount;
    }
    glUnmapNamedBuffer(glMesh.indirectBufferHandle);
}

void fillTextBuffer(const std::string& text, const Font& font, GuiVertex* vertexData)
{
    const glm::vec2& textureSize = font.textureSize;
    const auto& textureWidth = font.textureSize.x;
    const auto& textureHeight = font.textureSize.y;
    const auto& color = Color::white;
    const auto charactersCount = text.size();

    float x = 0;
    for(size_t i = 0; i < charactersCount; i++)
    {
        const auto ch = text[i];
        const auto& glyph = font.glyphs[ch];
        const auto glyphAdvancePx = FontUtils::advanceToPixels(glyph.advance);

        glm::vec2 size = glyph.size;
        glm::vec2 texOffset = glyph.textureOffset;
        const auto leftX = x + glyph.bearing.x;
        const auto rightX = x + size.x;
        const auto originY = size.y - glyph.bearing.y;
        const auto bottomY = -originY;
        const auto topY = size.y - originY;
        const auto leftS = texOffset.s / textureSize.x;
        const auto rightS = (texOffset.s + size.x) / textureSize.x;
        const auto topT = (texOffset.t + size.y) / textureSize.y;
        const auto bottomT = texOffset.t / textureSize.y;

        vertexData[i * VERTICES_FOR_CHARACTER_] = GuiVertex {color, glm::vec2 {leftX, topY}, {leftS, bottomT}};
        vertexData[i * VERTICES_FOR_CHARACTER_ + 1] = GuiVertex {color, {leftX, bottomY}, {leftS, topT}};
        vertexData[i * VERTICES_FOR_CHARACTER_ + 2] = GuiVertex {color, {rightX, topY}, {rightS, bottomT}};
        vertexData[i * VERTICES_FOR_CHARACTER_ + 3] = GuiVertex {color, {rightX, bottomY}, {rightS, topT}};

        x += glyphAdvancePx;
    }
}
