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
    mesh.vertexShaderHandle = EngineStateHelper::standardVertexShaderProgram(engineState).programHandle;
    mesh.fragmentShaderHandle = EngineStateHelper::standardColorFragmentShaderProgram(engineState).programHandle;

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

// TODO: Replace this shit with normal text rendering system!
void GuiPrimitiveFactory::makeTextQuads(
    EngineState& engineState,
    const std::string& text,
    const VersionedIndex fontIndex,
    const Transform& transform,
    const glm::vec4& color)
{
    const auto& font = engineState.fonts.get(fontIndex);

    float x = 0;
    for(const Font::char_type c: text)
    {
        const auto& glyph = font.glyphs[c];
        if (glyph.textureHandle == 0) continue;

        const auto& e = engineState.guiRegistry.create();
        auto quadPosition = transform.position;
        quadPosition.x += x;

        const auto& width = glyph.size.x;
        const auto& height = glyph.size.y;
        const auto& quad = GuiQuad::bottomLeftTopLeftTopRightBottomRight({
            GuiVertex {color, {0, 0}, {0, 1}},
            GuiVertex {color, {0, height}, {0, 0}},
            GuiVertex {color, {width, height}, {1, 0}},
            GuiVertex {color, {width, 0}, {1, 1}},
        });

        auto& mesh = makeEntityQuad(engineState, e, transform.withPosition(quadPosition), quad);
        mesh.fragmentShaderHandle =
            engineState.defaultShader.shaders[EngineInvariants::STANDARD_FONT_FRAGMENT_SHADER_INDEX].programHandle;

        addTexture(engineState, e, glyph.textureHandle);
        x += FontUtils::advanceToPixels(glyph.advance);
    }
}

