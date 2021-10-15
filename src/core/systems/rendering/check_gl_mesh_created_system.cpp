#include "check_gl_mesh_created_system.h"

#include <glm/mat4x4.hpp>

#include "../../components/gui_mesh.h"
#include "../../components/gl_mesh.h"

void CheckGlMeshCreatedSystem::update(entt::registry& ecsRegistry, const GlShaderState& shaderState)
{
    auto vaoHandle = shaderState.vertexArrayHandles[GlShaderState::TRIANGLES_ARRAY_INDEX];

    for (auto&& [e, mesh] : ecsRegistry.view<const GuiMesh>(entt::exclude<GlMesh>).each())
    {
        auto& glMesh = ecsRegistry.emplace<GlMesh>(e);
        auto& vertexVboHandle = glMesh.vboHandle;

        glCreateBuffers(1, &vertexVboHandle);
        glNamedBufferStorage(
            vertexVboHandle,
            mesh.vertices.size() * sizeof(GuiVertex),
            nullptr,
            GL_DYNAMIC_STORAGE_BIT
        );

        glBindVertexArray(vaoHandle);
        glBindBuffer(GL_ARRAY_BUFFER, vertexVboHandle);
        glVertexAttribPointer(GlMesh::VERTEX_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(GuiVertex), nullptr);
        glEnableVertexAttribArray(GlMesh::VERTEX_ATTRIB_INDEX);
    }
}
