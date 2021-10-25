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
        GuiVertex vertex;
        vertex.position = vertexPositions[i];
        vertex.color = color;

        vertices[i] = vertex;
    }

    vertices[0].uv = {0,0};
    vertices[1].uv = {0,1};
    vertices[2].uv = {1,1};
    vertices[3].uv = {1,0};

    return GuiQuad { vertices };
}