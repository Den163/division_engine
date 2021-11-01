#pragma once

#include "../../components/transform.h"
#include "../../components/gui_rect.h"
#include "../../components/gui_rect_color.h"
#include "../../utils/color.h"
#include "../../states/engine_state.h"
#include <entt/entt.hpp>

class GuiComposer
{
public:
    explicit GuiComposer(EngineState& engineState);

    entt::entity makeGuiElement(
        const Transform& transform = Transform::makeDefault());

    entt::entity makeGuiRect(
        const GuiRect& guiRect,
        const GuiRectColor& color = GuiRectColor::all(Color::white),
        const Transform transform = Transform::makeDefault());

private:
    EngineState& engineState;
};