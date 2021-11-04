#include "gl_gui_text_system.h"

#include "../../components/gl_mesh.h"
#include "../../components/gui_renderable.h"
#include "../../components/gui_text.h"
#include "../../utils/font_utils.h"
#include "../../utils/color.h"
#include "../../utils/math.h"

struct DrawArraysIndirectCommand
{
    GLuint count;
    GLuint primitivesCount;
    GLuint first;
    GLuint baseInstanceOffset;
};

constexpr size_t VERTICES_FOR_CHARACTER_ = 4;

// TODO Decouple it to different systems

static inline void applyIndirectBuffer(const GlMesh& glMesh, size_t charactersCount);
static inline void applyVertexBuffer(const std::string& text, const GlMesh& glMesh, const Font& font);
static inline void fillTextBuffer(const std::string& text, const Font& font, GuiVertex* vertexData);
static inline void applyModelViewProjectionMatrix(const GlMesh& glMesh, int divisor, const glm::mat4& mvpMatrix);

void GlGuiTextSystem::update(EngineState& engineState)
{
    auto& registry = engineState.guiRegistry;
    const auto& windowState = engineState.window;
    const auto& projectionMatrix = glm::ortho(0.f, (float) windowState.width, 0.f, (float) windowState.height);

    for (auto&&[e, glMesh, renderable, guiText] :
         registry.view<const GlMesh, const GuiRenderable, const GuiText>().each())
    {
        const auto text = guiText.text;
        const auto charactersCount = text.size();
        const auto verticesCount = charactersCount * VERTICES_FOR_CHARACTER_;
        const auto newBufferSize = verticesCount * sizeof(GuiVertex);
        const auto vboHandle = glMesh.vertexVboHandle;
        const auto fontIndex = guiText.font;
        const auto textSize = glm::vec2 { 0, guiText.fontHeight };
        const auto& modelMatrix = Math::transformMatrix(glm::vec3{0}, glm::identity<glm::quat>(), glm::vec3{1});

        glBindVertexArray(glMesh.vaoHandle);

        int32_t bufferSize = 0;
        glGetNamedBufferParameteriv(vboHandle, GL_BUFFER_SIZE, &bufferSize);

        if (bufferSize == 0)
        {
            glNamedBufferStorage(
                vboHandle, newBufferSize, nullptr, GL_MAP_WRITE_BIT);
            glNamedBufferStorage(
                glMesh.indirectBufferHandle, sizeof(DrawArraysIndirectCommand) * charactersCount, nullptr,
                GL_MAP_WRITE_BIT);
        }

        const auto& font = engineState.resources.fonts.get(fontIndex);
        applyIndirectBuffer(glMesh, charactersCount);
        applyVertexBuffer(text, glMesh, font);
        applyModelViewProjectionMatrix(glMesh, verticesCount, projectionMatrix * modelMatrix);

        const auto shaderPipelineHandle = renderable.shaderPipelineHandle;
        glBindVertexArray(glMesh.vaoHandle);
        glBindProgramPipeline(shaderPipelineHandle);
        glUseProgramStages(
            shaderPipelineHandle,
            GL_VERTEX_SHADER_BIT,
            renderable.vertexShaderProgramHandle);
        glUseProgramStages(
            shaderPipelineHandle,
            GL_FRAGMENT_SHADER_BIT,
            renderable.fragmentShaderProgramHandle);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindTexture(GL_TEXTURE_2D, font.textureHandle);
        glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, nullptr, charactersCount, sizeof(DrawArraysIndirectCommand));
    }
}

void applyIndirectBuffer(const GlMesh& glMesh, size_t charactersCount)
{
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, glMesh.indirectBufferHandle);
    auto* indirectsData = (DrawArraysIndirectCommand*) glMapNamedBuffer(glMesh.indirectBufferHandle, GL_WRITE_ONLY);
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

inline void applyVertexBuffer(const std::string& text, const GlMesh& glMesh, const Font& font)
{
    const auto vboHandle = glMesh.vertexVboHandle;
    GuiVertex* vertexData = (GuiVertex*) glMapNamedBuffer(vboHandle, GL_WRITE_ONLY);
    fillTextBuffer(text, font, vertexData);
    glUnmapNamedBuffer(vboHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glEnableVertexAttribArray(EngineInvariants::VERTEX_POSITION_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(
        EngineInvariants::VERTEX_POSITION_ATTRIBUTE_LOCATION,
        glm::vec3::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(GuiVertex),
        reinterpret_cast<void*>(offsetof(GuiVertex, position)));

    glEnableVertexAttribArray(EngineInvariants::VERTEX_COLOR_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(
        EngineInvariants::VERTEX_COLOR_ATTRIBUTE_LOCATION,
        glm::vec4::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(GuiVertex),
        reinterpret_cast<void*>(offsetof(GuiVertex, color)));

    glEnableVertexAttribArray(EngineInvariants::VERTEX_UV_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(
        EngineInvariants::VERTEX_UV_ATTRIBUTE_LOCATION,
        glm::vec2::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(GuiVertex),
        reinterpret_cast<void*>(offsetof(GuiVertex, uv)));
}

inline void fillTextBuffer(const std::string& text, const Font& font, GuiVertex* vertexData)
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
        const auto& size = glyph.size;
        const auto& texOffset = glyph.textureOffset;
        auto glyphAdvancePx = FontUtils::advanceToPixels(glyph.advance);
        auto leftX = x + glyph.bearing.x;
        auto rightX = x + size.x;
        auto originY = size.y - glyph.bearing.y;
        auto bottomY = -originY;
        auto topY = size.y - originY;

        vertexData[i * VERTICES_FOR_CHARACTER_] =
            GuiVertex{color, {leftX, topY}, glm::vec2 {texOffset.x, texOffset.y} / textureSize};
        vertexData[i * VERTICES_FOR_CHARACTER_ + 1] =
            GuiVertex{color, {leftX, bottomY}, glm::vec2 {texOffset.x, texOffset.y + size.y} / textureSize};
        vertexData[i * VERTICES_FOR_CHARACTER_ + 2] =
            GuiVertex{color, {rightX, topY}, glm::vec2 {texOffset.x + size.x, texOffset.y} / textureSize};
        vertexData[i * VERTICES_FOR_CHARACTER_ + 3] =
            GuiVertex{color, {rightX, bottomY}, glm::vec2{texOffset.x + size.x, texOffset.y + size.y} / textureSize};

        auto emptySpaceX = glyphAdvancePx - size.x;
        x += glyphAdvancePx;
    }
}

inline void applyModelViewProjectionMatrix(const GlMesh& glMesh, int divisor, const glm::mat4& mvpMatrix)
{
    const auto rows = glm::mat4::row_type::length();
    const auto columns = glm::mat4::col_type::length();

    glBindBuffer(GL_ARRAY_BUFFER, glMesh.modelViewProjectionVboHandle);
    glNamedBufferSubData(glMesh.modelViewProjectionVboHandle, 0, sizeof(glm::mat4), &mvpMatrix);

    for (size_t i = 0; i < rows; i++)
    {
        const auto attribId = EngineInvariants::MVP_MATRIX_ATTRIBUTE_LOCATION + i;
        glEnableVertexAttribArray(attribId);
        glVertexAttribPointer(
            attribId, columns, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (i * columns * sizeof(float)));
        glVertexAttribDivisor(attribId, divisor);
    }
}
