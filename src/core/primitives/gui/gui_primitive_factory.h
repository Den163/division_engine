#pragma once

#include <array>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <tuple>

#include "../../components/gui_mesh.h"
#include "gui_triangle.h"
#include "gui_quad.h"

namespace GuiPrimitiveFactory
{
    std::tuple<const entt::entity&, GuiMesh&> createMeshEntity(entt::registry& registry, const Transform& transform);

    std::tuple<const entt::entity&, GuiMesh&> createTriangle(
        entt::registry& guiRegistry,
        const Transform& transform,
        const GuiTriangle& triangle);

    std::tuple<const entt::entity&, GuiMesh&> createQuad(
        entt::registry& guiRegistry,
        const Transform& transform,
        const GuiQuad& quad);

}