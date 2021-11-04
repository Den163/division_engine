#pragma once

#include <glm/vec4.hpp>

struct GuiRectColor
{
    glm::vec4 bottomLeft;
    glm::vec4 topLeft;
    glm::vec4 topRight;
    glm::vec4 bottomRight;

    static GuiRectColor all(const glm::vec4& color) { return GuiRectColor { color, color, color, color }; }
};