#include "on_gui_mesh_entity_created_event_system.h"
#include "../components/gl_mesh.h"
#include "../components/gui_mesh.h"
#include "../events/create_gui_mesh_entity.h"
#include "../../utils/shader_query.h"

void OnGuiMeshEntityCreatedEventSystem::update(entt::registry& registry, const GlShaderState& shaderState)
{
    for (auto&& [e, guiMesh] : registry.view<const GuiMesh, const CreateGuiMeshEntity>().each())
    {
        auto& glMesh = registry.emplace<GlMesh>(e);

        ShaderQuery { shaderState.programHandle, shaderState.vaoHandle}
            .createEmptyBuffer(glMesh.vertexVboHandle, guiMesh.vertices);

        registry.remove<CreateGuiMeshEntity>(e);
    }
}
