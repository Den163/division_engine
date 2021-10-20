#include "on_gui_mesh_entity_destroyed_event_system.h"

#include "../components/gl_mesh.h"
#include "../events/destroy_gui_mesh_entity.h"

void OnGuiMeshEntityDestroyedEventSystem::update(EngineState& engineState)
{
    auto& registry = engineState.guiRegistry;

    for (auto&& [e, glMesh] : registry.view<const GlMesh, const DestroyGuiMeshEntity>().each())
    {
        glBindVertexArray(glMesh.vaoHandle);
        glDeleteBuffers(1, &glMesh.vertexVboHandle);
        glDeleteBuffers(1, &glMesh.modelViewProjectionVboHandle);
        glDeleteVertexArrays(1, &glMesh.vaoHandle);

        registry.destroy(e);
    }
}
