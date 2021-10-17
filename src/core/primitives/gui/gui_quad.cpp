#include "gui_quad.h"

GuiQuad GuiQuad::create(float width, float height, const glm::vec4& color)
{
    std::array<glm::vec3, VERTICES_COUNT> vertexPositions = {
        glm::vec3 {0.f},
        {0.f, height, 0.f},
        {width, height, 0.f},
        {width, 0.f, 0.f}
    };

    return create(vertexPositions, color);
}

GuiQuad GuiQuad::create(const std::array<glm::vec3, VERTICES_COUNT>& vertexPositions, const glm::vec4& color)
{
    std::array<GuiVertex, VERTICES_COUNT> vertices {};
    for (size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i] = GuiVertex { vertexPositions[i], color };
    }

    return GuiQuad { vertices };
}