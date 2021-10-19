#include "check_gl_mesh_created_system.h"

#include <glm/mat4x4.hpp>

#include "../../components/gui_mesh.h"
#include "../../components/gl_mesh.h"
#include "../../../utils/shader_query.h"

void CheckGlMeshCreatedSystem::update(entt::registry& ecsRegistry, const GlShaderState& shaderState)
{
    auto vaoHandle = shaderState.vertexArrayHandles[GlShaderState::TRIANGLES_ARRAY_INDEX];

    for (auto&& [e, mesh] : ecsRegistry.view<const GuiMesh>(entt::exclude<GlMesh>).each())
    {
        auto& glMesh = ecsRegistry.emplace<GlMesh>(e);

        ShaderQuery { shaderState.programHandle, vaoHandle }
            .createEmptyBuffer(glMesh.vertexVboHandle, mesh.vertices);
    }
}
