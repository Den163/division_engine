#pragma once

#include <array>
#include "gui_vertex.h"

struct GuiQuad
{
    static GuiQuad atBottomLeft(
        const glm::vec2& pos,
        const glm::vec2& size,
        const glm::vec4& color);

    static GuiQuad bottomLeftTopLeftTopRightBottomRight(const std::array<GuiVertex, 4>& vertices);

    GuiVertex topLeft;
    GuiVertex bottomLeft;
    GuiVertex topRight;
    GuiVertex bottomRight;
};