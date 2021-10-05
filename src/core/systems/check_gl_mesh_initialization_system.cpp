#include "check_gl_mesh_initialization_system.h"

#include "../components/mesh.h"
#include "../components/gl_mesh.h"

void CheckGlMeshInitializationSystem::update(entt::registry& ecsRegistry, GlShaderState& shaderState)
{
    auto vaoHandle = shaderState.vertexArrayHandles[GlShaderState::TRIANGLES_ARRAY_INDEX];

    for (auto&& [e, mesh] : ecsRegistry.view<Mesh>(entt::exclude<GlMesh>).each())
    {
        auto& glMesh = ecsRegistry.emplace<GlMesh>(e);
        auto& vboHandle = glMesh.vboHandle;

        glCreateBuffers(1, &vboHandle);
        glNamedBufferStorage(
            vboHandle,
            mesh.vertices.size() * sizeof(Mesh::Vertex),
            nullptr,
            GL_DYNAMIC_STORAGE_BIT
        );

        glBindVertexArray(vaoHandle);
        glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
        glVertexAttribPointer(
            GlMesh::VERTEX_POSITION_ATTRIB_INDEX,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Mesh::Vertex),
            nullptr
        );
        glEnableVertexAttribArray(GlMesh::VERTEX_POSITION_ATTRIB_INDEX);
    }
}
