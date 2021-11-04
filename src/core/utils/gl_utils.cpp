#include "gl_utils.h"
#include "engine_invariants.h"
#include "../primitives/gui/gui_vertex.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

void GlUtils::enableGuiVertexAttributes(GLuint vaoHandle, GLuint vertexVboHandle)
{
    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVboHandle);
    glEnableVertexAttribArray(EngineInvariants::VERTEX_POSITION_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(
        EngineInvariants::VERTEX_POSITION_ATTRIBUTE_LOCATION,
        glm::vec2::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(GuiVertex),
        reinterpret_cast<void*>(offsetof(GuiVertex, position)));

    glEnableVertexAttribArray(EngineInvariants::VERTEX_COLOR_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(
        EngineInvariants::VERTEX_COLOR_ATTRIBUTE_LOCATION,
        glm::vec4::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(GuiVertex),
        reinterpret_cast<void*>(offsetof(GuiVertex, color)));

    glEnableVertexAttribArray(EngineInvariants::VERTEX_UV_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(
        EngineInvariants::VERTEX_UV_ATTRIBUTE_LOCATION,
        glm::vec2::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(GuiVertex),
        reinterpret_cast<void*>(offsetof(GuiVertex, uv)));
}