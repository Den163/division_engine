#include "on_gui_mesh_entity_created_event_system.h"
#include "../components/gl_mesh.h"
#include "../components/gui_mesh.h"
#include "../events/create_gui_mesh_entity.h"

void OnGuiMeshEntityCreatedEventSystem::update(entt::registry& registry, const GlShaderState& shaderState)
{
    for (auto&& [e, guiMesh] : registry.view<const GuiMesh, const CreateGuiMeshEntity>().each())
    {
        auto& glMesh = registry.emplace<GlMesh>(e);

        glUseProgram(shaderState.programHandle);
        glBindVertexArray(shaderState.vaoHandle);
        glCreateBuffers(1, &glMesh.vertexVboHandle);
        glNamedBufferStorage(
            glMesh.vertexVboHandle, guiMesh.vertices.size() * sizeof(GuiVertex), nullptr, GL_DYNAMIC_STORAGE_BIT);

        glCreateBuffers(1, &glMesh.modelViewProjectionVboHandle);
        glNamedBufferStorage(glMesh.modelViewProjectionVboHandle, sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT);

        registry.remove<CreateGuiMeshEntity>(e);
    }
}
