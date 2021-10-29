#pragma once

#include <array>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <string>
#include <tuple>

#include "../../components/gui_mesh.h"
#include "../../states/engine_state.h"
#include "../../utils/color.h"
#include "gui_triangle.h"
#include "gui_quad.h"

namespace GuiPrimitiveFactory
{
    GuiMesh& makeEntityMesh(
        EngineState& engineState, const entt::entity& entity, const Transform& transform);

    void deleteMeshEntity(EngineState& engineState, const entt::entity& entity);

    GuiMesh& makeEntityTriangle(
        EngineState& engineState,
        const entt::entity& entity,
        const Transform& transform,
        const GuiTriangle& triangle);

    GuiMesh& makeEntityQuad(
        EngineState& engineState, const entt::entity& entity, const Transform& transform, const GuiQuad& quad);

    void addTexture(EngineState& engineState, const entt::entity& entity, uint32_t textureIndex);

    void makeTextQuads(
        EngineState& engineState,
        const std::string& text,
        const VersionedIndex fontIndex,
        const Transform& transform = Transform::makeDefault(),
        const glm::vec4& color = Color::white);
}