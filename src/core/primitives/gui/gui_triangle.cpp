#include "gui_triangle.h"

GuiTriangle GuiTriangle::create(const std::array <glm::vec3, VERTICES_COUNT>& verticesPositions, const glm::vec4& color)
{
    std::array<GuiVertex, VERTICES_COUNT> vertices {};
    for (size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i] = GuiVertex { verticesPositions[i], color };
    }

    return GuiTriangle { vertices };
}
