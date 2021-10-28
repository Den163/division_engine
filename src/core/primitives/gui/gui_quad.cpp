#include "gui_quad.h"

GuiQuad GuiQuad::atBottomLeft(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
{
    return GuiQuad
    {
        .topLeft = GuiVertex  { color, { pos.x, pos.y + size.y }, { 0, 1 }  },
        .bottomLeft = GuiVertex { color, pos, { 0, 0 }  },
        .topRight = GuiVertex { color, {pos.x + size.x, pos.y + size.y }, { 1, 1 } },
        .bottomRight = GuiVertex { color, { pos.x + size.x, pos.y }, { 1, 0 } }
    };
}

GuiQuad GuiQuad::bottomLeftTopLeftTopRightBottomRight(const std::array<GuiVertex, 4>& vertices)
{
    return GuiQuad
    {
        .topLeft = vertices[1],
        .bottomLeft = vertices[0],
        .topRight = vertices[2],
        .bottomRight = vertices[3]
    };
}
