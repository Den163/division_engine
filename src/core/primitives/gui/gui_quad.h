#pragma once

#include <array>
#include "gui_vertex.h"

struct GuiQuad
{
    static constexpr size_t VERTICES_COUNT = 4;

    static GuiQuad create(float width, float height, const glm::vec4& color);
    static GuiQuad create(const std::array<glm::vec2, VERTICES_COUNT>& vertexPositions, const glm::vec4& color);

    std::array<GuiVertex, VERTICES_COUNT> vertices;
};