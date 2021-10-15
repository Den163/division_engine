#include "primitive_factory.h"

#include "../core/components/position.h"
#include "../core/components/rotation.h"
#include "../core/components/scale.h"

std::tuple<const entt::entity&, GuiMesh&> PrimitiveFactory::createMeshEntity(
    entt::registry& ecsRegistry,
    const glm::vec3& position,
    const glm::quat& rotation,
    const glm::vec3& scale)
{
    const auto& e = ecsRegistry.create();
    auto& mesh = ecsRegistry.emplace<GuiMesh>(e);

    auto& newPos = ecsRegistry.emplace<Position>(e);
    auto& newRot = ecsRegistry.emplace<Rotation>(e);
    auto& newScale = ecsRegistry.emplace<Scale>(e);

    newPos.value = position;
    newRot.value = rotation;
    newScale.value = scale;

    return {e, mesh};
}

std::tuple<const entt::entity&, GuiMesh&> PrimitiveFactory::createTriangle(
    entt::registry& ecsRegistry,
    const Triangle& triangle)
{
    const auto& transform = triangle.transform;
    auto [e, mesh] = createMeshEntity(ecsRegistry, transform.position, transform.rotation, transform.scale);

    mesh.renderShape = RenderShape::Triangle;
    mesh.vertices = std::vector<GuiVertex> { triangle.vertices.begin(), triangle.vertices.end() };

    return {e, mesh};
}

std::tuple<const entt::entity&, GuiMesh&> PrimitiveFactory::createTriangle(
    entt::registry& ecsRegistry,
    const Transform& transform,
    const std::array<glm::vec3, 3>& vertexPositions,
    const glm::vec4& color)
{
    std::array<GuiVertex, 3> vertices {};
    for (auto i = 0; i < vertices.size(); i++) {
        vertices[i] = GuiVertex { vertexPositions[i], color };
    }

    return createTriangle(ecsRegistry, Triangle {transform, vertices});
}

