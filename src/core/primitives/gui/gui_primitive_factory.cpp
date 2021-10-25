#include "gui_primitive_factory.h"

#include "../../components/position.h"
#include "../../components/rotation.h"
#include "../../components/scale.h"
#include "../../states/gl_shader_pipeline_state.h"
#include "../../events/gui_mesh_created.h"
#include "../../events/gui_mesh_destroyed.h"
#include "../../components/gl_texture.h"

GuiMesh& GuiPrimitiveFactory::makeEntityMesh(
    entt::registry& registry, const entt::entity& entity, const Transform& transform)
{
    auto& mesh = registry.emplace<GuiMesh>(entity);
    auto& newPos = registry.emplace<Position>(entity);
    auto& newRot = registry.emplace<Rotation>(entity);
    auto& newScale = registry.emplace<Scale>(entity);

    newPos.value = transform.position;
    newRot.value = transform.rotation;
    newScale.value = transform.scale;

    registry.emplace<GuiMeshCreated>(entity);

    return mesh;
}


void GuiPrimitiveFactory::deleteMeshEntity(entt::registry& registry, const entt::entity& entity)
{
    registry.emplace<GuiMeshDestroyed>(entity);
}

GuiMesh& GuiPrimitiveFactory::makeEntityTriangle(
    entt::registry& guiRegistry, const entt::entity& entity, const Transform& transform, const GuiTriangle& triangle)
{
    auto& mesh = makeEntityMesh(guiRegistry, entity, transform);
    mesh.renderMode = RenderMode::Triangles;
    mesh.vertices = std::vector<GuiVertex> { triangle.vertices.begin(), triangle.vertices.end() };

    return mesh;
}

GuiMesh& GuiPrimitiveFactory::makeEntityQuad(
    entt::registry& guiRegistry, const entt::entity& entity, const Transform& transform, const GuiQuad& quad)
{
    auto& mesh = makeEntityMesh(guiRegistry, entity, transform);
    mesh.renderMode = RenderMode::TrianglesFan;
    mesh.vertices = std::vector<GuiVertex> { quad.vertices.begin(), quad.vertices.end() };

    return mesh;
}

void GuiPrimitiveFactory::addTexture(entt::registry& registry, const entt::entity& entity, uint32_t handle)
{
    auto& tex = registry.emplace<GlTexture>(entity);
    tex.handle = handle;
}

