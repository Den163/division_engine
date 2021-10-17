#include "gui_primitive_factory.h"

#include "../core/components/position.h"
#include "../core/components/rotation.h"
#include "../core/components/scale.h"

std::tuple<const entt::entity&, GuiMesh&> GuiPrimitiveFactory::createMeshEntity(
    entt::registry& registry,
    const Transform& transform)
{
    const auto& e = registry.create();
    auto& mesh = registry.emplace<GuiMesh>(e);

    auto& newPos = registry.emplace<Position>(e);
    auto& newRot = registry.emplace<Rotation>(e);
    auto& newScale = registry.emplace<Scale>(e);

    newPos.value = transform.position;
    newRot.value = transform.rotation;
    newScale.value = transform.scale;

    return {e, mesh};
}

std::tuple<const entt::entity&, GuiMesh&> GuiPrimitiveFactory::createTriangle(
    entt::registry& guiRegistry,
    const Transform& transform,
    const GuiTriangle& triangle)
{
    auto [e, mesh] = createMeshEntity(guiRegistry, transform);
    mesh.renderShape = RenderMode::Triangles;
    mesh.vertices = std::vector<GuiVertex> { triangle.vertices.begin(), triangle.vertices.end() };

    return {e, mesh};
}

std::tuple<const entt::entity&, GuiMesh&> GuiPrimitiveFactory::createQuad(
    entt::registry& guiRegistry,
    const Transform& transform,
    const GuiQuad& quad)
{
    auto [e, mesh] = createMeshEntity(guiRegistry, transform);
    mesh.renderShape = RenderMode::TrianglesFan;
    mesh.vertices = std::vector<GuiVertex> { quad.vertices.begin(), quad.vertices.end() };

    return {e, mesh};
}
