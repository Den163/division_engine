#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

struct Mesh
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec4 color;
    };

    std::vector<Vertex> vertices;
};