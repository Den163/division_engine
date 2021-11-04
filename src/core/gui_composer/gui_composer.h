#pragma once

#include "../states/engine_state.h"
#include "../components/transform.h"
#include <entt/entt.hpp>

#include "../components/gui_material.h"
#include "../components/gui_text.h"
#include "../components/position.h"
#include "../components/rotation.h"
#include "../components/scale.h"

struct GlMesh;

struct GuiComposer
{
    EngineState& engineState;

    inline static constexpr auto gui_element_types = entt::get<GlMesh, GuiMaterial, Position, Rotation, Scale>;
    inline static constexpr auto gui_text_types = gui_element_types + entt::get<GuiText>;

    GuiComposer(EngineState& engineState) : engineState(engineState) {}

    entt::entity makeGuiElement(const GuiMaterial& material, const Transform& transform = Transform::makeDefault());

    inline entt::entity makeGuiText(const GuiText& guiText, const Transform& transform = Transform::makeDefault())
    {
        return makeGuiText(guiText, defaultFontMaterial(), transform);
    }
    entt::entity makeGuiText(
        const GuiText& guiText,
        const GuiMaterial& material,
        const Transform& transform = Transform::makeDefault());

    GuiMaterial defaultColoredMaterial() const;
    GuiMaterial defaultTextureMaterial() const;
    GuiMaterial defaultFontMaterial() const;


    inline constexpr entt::registry& guiRegistry() { return engineState.guiRegistry; }
};

