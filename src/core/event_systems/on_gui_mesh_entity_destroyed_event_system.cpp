#include "on_gui_mesh_entity_destroyed_event_system.h"

#include "../components/gl_mesh.h"
#include "../events/destroy_gui_mesh_entity.h"
#include "../../utils/shader_query.h"

void OnGuiMeshEntityDestroyedEventSystem::update(entt::registry& registry, const GlShaderState& shaderState)
{
    for (auto&& [e, glMesh] : registry.view<const GlMesh, const DestroyGuiMeshEntity>().each())
    {
        ShaderQuery { shaderState.programHandle, shaderState.vaoHandle }
            .deleteBuffer(glMesh.vertexVboHandle);

        registry.destroy(e);
    }
}
