#pragma once

#include <array>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <tuple>

#include "../core/components/gui_mesh.h"
#include "../core/primitives/gui/gui_triangle.h"
#include "../core/primitives/gui/gui_quad.h"

namespace GuiPrimitiveFactory
{
    std::tuple<const entt::entity&, GuiMesh&> createMeshEntity(
        entt::registry& ecsRegistry,
        const glm::vec3& position = glm::vec3 {0},
        const glm::quat& rotation = glm::quat {glm::vec3 {0}},
        const glm::vec3& scale = glm::vec3 {1}
                                                              );

    void deleteMeshEntity(entt::registry& ecsRegistry, const entt::entity& entity);

    std::tuple<const entt::entity&, GuiMesh&> createTriangle(
        entt::registry& ecsRegistry,
        const Transform& transform,
        const std::array<glm::vec3, 3>& vertices,
        const glm::vec4& color);

    std::tuple<const entt::entity&, GuiMesh&> createTriangle(
        entt::registry& ecsRegistry,
        const Transform& transform,
        const GuiTriangle& triangle);

    std::tuple<const entt::entity&, GuiMesh&> createQuad(
        entt::registry& guiRegistry,
        const Transform& transform,
        const std::array<glm::vec3, 4>& vertices,
        const glm::vec4& color);

    std::tuple<const entt::entity&, GuiMesh&> createQuad(
        entt::registry& guiRegistry,
        const Transform& transform,
        const GuiQuad& quad);
}