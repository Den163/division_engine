#include "on_gui_mesh_entity_created_event_system.h"

#include "../utils/engine_state_helper.h"
#include "../components/gl_mesh.h"
#include "../events/gui_mesh_created.h"
#include "../utils/gl_utils.h"

void OnGuiMeshEntityCreatedEventSystem::preRender(EngineState& engineState)
{
    auto& registry = engineState.guiRegistry;

    for (auto&& [e] : registry.view<const GuiMeshCreated>().each())
    {
        auto& glMesh = registry.emplace<GlMesh>(e);
        const auto& shaderPipelineHandle = EngineStateHelper::standardShaderPipeline(engineState);

        glMesh.verticesCount = 0;
        glMesh.primitivesCount = 0;
        glCreateVertexArrays(1, &glMesh.vaoHandle);
        glCreateBuffers(1, &glMesh.vertexVboHandle);
        glCreateBuffers(1, &glMesh.indirectBufferHandle);

        glCreateBuffers(1, &glMesh.modelViewProjectionVboHandle);
        glNamedBufferStorage(glMesh.modelViewProjectionVboHandle, sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT);

        GlUtils::enableGuiVertexAttributes(glMesh.vaoHandle, glMesh.vertexVboHandle);
    }
}
