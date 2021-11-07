#include "gl_utils.h"

#include <components/gl_draw_indirect_command.h>
#include <primitives/gui/gui_vertex.h>
#include <utils/engine_invariants.h>

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

void GlUtils::makeGuiVertexBufferStorage(const GlMesh& glMesh, const GuiMesh& guiMesh)
{
    const auto primitivesCount = guiMesh.primitivesCount;
    const auto verticesCount = guiMesh.verticesCount;
    const auto verticesPerPrimitive = verticesCount / primitivesCount;
    const auto indirectBufferHandle = glMesh.indirectBufferHandle;
    const auto needSize = verticesCount * sizeof(GuiVertex);
    
    assert(needSize > 0);

    glBindVertexArray(glMesh.vaoHandle);

    int32_t bufferSize = 0;
    glGetNamedBufferParameteriv(glMesh.vertexVboHandle, GL_BUFFER_SIZE, &bufferSize);

    if (needSize > bufferSize || bufferSize == 0)
    {
        glNamedBufferStorage(glMesh.vertexVboHandle, static_cast<GLsizeiptr>(needSize), nullptr, GL_MAP_WRITE_BIT);
        glNamedBufferStorage(
            indirectBufferHandle,
            static_cast<GLsizeiptr>(sizeof(GlDrawArraysIndirectCommand) * primitivesCount),
            nullptr,
            GL_MAP_WRITE_BIT
        );
    }

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBufferHandle);
    auto* indirectsData = (GlDrawArraysIndirectCommand*) glMapNamedBuffer(indirectBufferHandle, GL_WRITE_ONLY);
    for (size_t i = 0; i < primitivesCount; i++)
    {
        auto& indirect = indirectsData[i];
        indirect.first = i * verticesPerPrimitive;
        indirect.count = verticesPerPrimitive;
        indirect.baseInstanceOffset = 0;
        indirect.primitivesCount = primitivesCount;
    }
    glUnmapNamedBuffer(indirectBufferHandle);
}
