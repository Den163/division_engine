#include "on_gui_mesh_entity_destroyed_event_system.h"

#include "../components/gl_mesh.h"
#include "../events/gui_mesh_destroyed.h"

void OnGuiMeshEntityDestroyedEventSystem::preRender(EngineState& engineState)
{
    auto& registry = engineState.guiRegistry;

    for (auto&& [e, glMesh] : registry.view<const GlMesh, const GuiMeshDestroyed>().each())
    {
        glBindVertexArray(glMesh.vaoHandle);
        glDeleteBuffers(1, &glMesh.vertexVboHandle);
        glDeleteBuffers(1, &glMesh.modelViewProjectionVboHandle);
        glDeleteVertexArrays(1, &glMesh.vaoHandle);

        registry.destroy(e);
    }
}
