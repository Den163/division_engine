#include "check_gl_mesh_destroyed_system.h"

#include "../../components/gui_mesh.h"
#include "../../components/gl_mesh.h"

void CheckGlMeshDestroyedSystem::update(entt::registry& ecsRegistry, const GlShaderState& shaderState)
{
    for (auto&& [e, glMesh] : ecsRegistry.view<const GlMesh>(entt::exclude<GuiMesh>).each())
    {
        glBindVertexArray(shaderState.vertexArrayHandles[GlShaderState::TRIANGLES_ARRAY_INDEX]);
        glDeleteBuffers(1, &glMesh.vertexVboHandle);

        ecsRegistry.remove<GlMesh>(e);
    }
}
