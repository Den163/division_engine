#pragma once

#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

#include "../core/components/mesh.h"

#include <tuple>

namespace PrimitiveFactory
{
std::tuple<const entt::entity&, Mesh&> createMeshEntity(
    entt::registry& ecsRegistry,
    const glm::vec3& position = glm::vec3 {0},
    const glm::quat& rotation = glm::quat {glm::vec3 {0}},
    const glm::vec3& scale = glm::vec3 {1}
);

void deleteMeshEntity(entt::registry& ecsRegistry, const entt::entity& entity);
}