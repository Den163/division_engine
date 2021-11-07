#include "on_gui_mesh_entity_created_event_system.h"

#include <components/gl_mesh.h>
#include <components/gui_mesh.h>
#include <events/gui_mesh_create_event.h>
#include <utils/gl_utils.h>

void OnGuiMeshEntityCreatedEventSystem::preRender(EngineState& engineState)
{
    auto& registry = engineState.guiRegistry;

    for (auto&& [e] : registry.view<const GuiMeshCreateEvent>().each())
    {
        {
            auto& guiMesh = registry.emplace<GuiMesh>(e);
            guiMesh.primitivesCount = 0;
            guiMesh.verticesCount = 0;
        }
        {
            auto& glMesh = registry.emplace<GlMesh>(e);

            glCreateVertexArrays(1, &glMesh.vaoHandle);
            glCreateBuffers(1, &glMesh.vertexVboHandle);
            glCreateBuffers(1, &glMesh.indirectBufferHandle);

            glCreateBuffers(1, &glMesh.modelViewProjectionVboHandle);
            glNamedBufferStorage(
                glMesh.modelViewProjectionVboHandle,
                sizeof(glm::mat4),
                nullptr,
                GL_DYNAMIC_STORAGE_BIT);

            GlUtils::enableGuiVertexAttributes(glMesh.vaoHandle, glMesh.vertexVboHandle);

            glMesh.renderMode = GL_TRIANGLE_STRIP;
        }

    }
}
