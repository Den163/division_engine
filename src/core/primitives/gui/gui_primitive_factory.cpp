#include "gui_primitive_factory.h"

#include "../../components/position.h"
#include "../../components/rotation.h"
#include "../../components/scale.h"
#include "../../events/gui_mesh_created.h"
#include "../../events/gui_mesh_destroyed.h"
#include "../../components/gl_texture.h"
#include "../../utils/engine_state_helper.h"
#include "../../utils/font_utils.h"

GuiMesh& GuiPrimitiveFactory::makeEntityMesh(
    EngineState& engineState, const entt::entity& entity, const Transform& transform)
{
    auto& registry = engineState.guiRegistry;
    auto& mesh = registry.emplace<GuiMesh>(entity);
    mesh.shaderPipelineHandle = EngineStateHelper::standardShaderPipeline(engineState).handle;
    mesh.vertexShaderProgramHandle = EngineStateHelper::standardVertexShaderProgram(engineState).programHandle;
    mesh.fragmentShaderProgramHandle = EngineStateHelper::standardColorFragmentShaderProgram(engineState).programHandle;

    auto& newPos = registry.emplace<Position>(entity);
    auto& newRot = registry.emplace<Rotation>(entity);
    auto& newScale = registry.emplace<Scale>(entity);

    newPos.value = transform.position;
    newRot.value = transform.rotation;
    newScale.value = transform.scale;

    registry.emplace<GuiMeshCreated>(entity);

    return mesh;
}


void GuiPrimitiveFactory::deleteMeshEntity(EngineState& engineState, const entt::entity& entity)
{
    engineState.guiRegistry.emplace<GuiMeshDestroyed>(entity);
}

GuiMesh& GuiPrimitiveFactory::makeEntityTriangle(
    EngineState& engineState, const entt::entity& entity, const Transform& transform, const GuiTriangle& triangle)
{
    auto& mesh = makeEntityMesh(engineState, entity, transform);
    mesh.renderMode = RenderMode::Triangles;
    mesh.vertices = std::vector<GuiVertex> { triangle.vertices.begin(), triangle.vertices.end() };

    return mesh;
}

GuiMesh& GuiPrimitiveFactory::makeEntityQuad(
    EngineState& engineState, const entt::entity& entity, const Transform& transform, const GuiQuad& quad)
{
    auto& mesh = makeEntityMesh(engineState, entity, transform);
    mesh.renderMode = RenderMode::TrianglesStrip;
    mesh.vertices = std::vector<GuiVertex> { &quad.topLeft, &quad.bottomRight + 1 };

    return mesh;
}

void GuiPrimitiveFactory::addTexture(EngineState& engineState, const entt::entity& entity, uint32_t handle)
{
    auto& tex = engineState.guiRegistry.emplace<GlTexture>(entity);
    tex.handle = handle;
}

void GuiPrimitiveFactory::makeTextQuads(
    EngineState& engineState,
    const std::string& text,
    const VersionedIndex fontIndex,
    const Transform& transform,
    const glm::vec4& color)
{
    const auto& font = engineState.resources.fonts.get(fontIndex);
    const glm::vec2& textureSize = font.textureSize;
    const auto& textureWidth = font.textureSize.x;
    const auto& textureHeight = font.textureSize.y;
    const auto& textSize = text.size();
    const auto verticesInQuad = 4;

    const auto& e = engineState.guiRegistry.create();
    auto& fontMesh = makeEntityMesh(engineState, e, transform);
    auto& vertices = fontMesh.vertices;
    vertices.resize(textSize * verticesInQuad * 2);

    fontMesh.renderMode = RenderMode::TrianglesStrip;
    fontMesh.fragmentShaderProgramHandle =
        engineState.defaultShader.fontFragment().programHandle;
    addTexture(engineState, e, font.textureHandle);

    float x = 0;
    auto renderedVertices = 0;
    for(size_t i = 0; i < textSize; i++)
    {
        const auto c = text[i];
        const auto& glyph = font.glyphs[c];
        const auto& size = glyph.size;
        const auto& offset = glyph.offset;
        auto glyphAdvancePx = FontUtils::advanceToPixels(glyph.advance);
        auto leftX = x;
        auto rightX = x + size.x;
        auto bottomY = 0;
        auto topY = size.y;

        vertices[renderedVertices++] =
            GuiVertex{color, {leftX, topY}, glm::vec2 {offset.x, offset.y} / textureSize};
        vertices[renderedVertices++] =
            GuiVertex{color, {leftX, bottomY}, glm::vec2 {offset.x, offset.y + size.y} / textureSize};
        vertices[renderedVertices++] =
            GuiVertex{color, {rightX, topY}, glm::vec2 {offset.x + size.x, offset.y} / textureSize};
        vertices[renderedVertices++] =
            GuiVertex{color, {rightX, bottomY}, glm::vec2{offset.x + size.x, offset.y + size.y} / textureSize};

        auto emptySpaceX = glyphAdvancePx - size.x;
        if (emptySpaceX > 0)
        {
            auto transparency = Color::transparency;
            vertices[renderedVertices++] = GuiVertex{transparency, {rightX, topY}, glm::vec2{0}};
            vertices[renderedVertices++] = GuiVertex{transparency, {rightX, bottomY}, glm::vec2{0}};
            vertices[renderedVertices++] = GuiVertex{transparency,{rightX + emptySpaceX, topY}, glm::vec2{0}};
            vertices[renderedVertices++] = GuiVertex{transparency,{rightX + emptySpaceX, bottomY}, glm::vec2{0}};
        }

        x += glyphAdvancePx;
    }

    vertices.resize(renderedVertices);
}

