#pragma once

#include <glad/gl.h>
#include <vector>

#include <primitives/gui/gui_vertex.h>

enum class RenderMode
{
    Points = GL_POINTS,

    Lines = GL_LINES,
    LinesStrip = GL_LINE_STRIP,
    LinesStripAdjacency = GL_LINE_STRIP_ADJACENCY,
    LinesLoop = GL_LINE_LOOP,

    Triangles = GL_TRIANGLES,
    TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
    TrianglesStrip = GL_TRIANGLE_STRIP,
    TrianglesStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
    TrianglesFan = GL_TRIANGLE_FAN,

    Patches = GL_PATCHES
};