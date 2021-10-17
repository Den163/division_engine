#pragma once

#include <array>

#include "gui_vertex.h"
#include "../../components/transform.h"

struct GuiTriangle
{
    static constexpr size_t VERTICES_COUNT = 3;
    static GuiTriangle create(const std::array<glm::vec3, VERTICES_COUNT>& verticesPositions, const glm::vec4& color)
    {
        std::array<GuiVertex, VERTICES_COUNT> vertices {};
        for (size_t i = 0; i < vertices.size(); i++)
        {
            vertices[i] = GuiVertex { verticesPositions[i], color };
        }

        return GuiTriangle { vertices };
    }

    std::array<GuiVertex, VERTICES_COUNT> vertices;
};