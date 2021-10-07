#include "primitive_factory.h"

#include "../core/components/position.h"
#include "../core/components/rotation.h"
#include "../core/components/scale.h"

std::tuple<const entt::entity&, Mesh&> PrimitiveFactory::createMeshEntity(
    entt::registry& ecsRegistry,
    const glm::vec3& position,
    const glm::quat& rotation,
    const glm::vec3& scale)
{
    const auto& e = ecsRegistry.create();
    auto& mesh = ecsRegistry.emplace<Mesh>(e);

    auto& newPos = ecsRegistry.emplace<Position>(e);
    auto& newRot = ecsRegistry.emplace<Rotation>(e);
    auto& newScale = ecsRegistry.emplace<Scale>(e);

    newPos.value = position;
    newRot.value = rotation;
    newScale.value = scale;

    return { e, mesh };
}
