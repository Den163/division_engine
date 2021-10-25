#pragma once

#include <array>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <tuple>

#include "../../components/gui_mesh.h"
#include "../../components/texture_2d.h"
#include "gui_triangle.h"
#include "gui_quad.h"

namespace GuiPrimitiveFactory
{
    GuiMesh& makeEntityMesh(
        entt::registry& registry, const entt::entity& entity, const Transform& transform);

    void deleteMeshEntity(entt::registry& registry, const entt::entity& entity);

    GuiMesh& makeEntityTriangle(
        entt::registry& guiRegistry,
        const entt::entity& entity,
        const Transform& transform,
        const GuiTriangle& triangle);

    GuiMesh& makeEntityQuad(
        entt::registry& guiRegistry, const entt::entity& entity, const Transform& transform, const GuiQuad& quad);

    Texture2d& addTexture(entt::registry& registry, const entt::entity& entity, uint32_t textureIndex);

}