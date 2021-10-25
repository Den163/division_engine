#include "gl_render_gui_mesh_system.h"

#include "../../components/gl_mesh.h"
#include "../../components/gui_mesh.h"
#include "../../../utils/engine_state_helper.h"
#include "../../components/texture_2d.h"

void GlRenderGuiMeshSystem::update(EngineState& engineState)
{
    auto& registry = engineState.guiRegistry;

    for (auto&& [e, glMesh, mesh] : registry.view<const GlMesh, const GuiMesh>().each())
    {
        const auto shaderPipelineHandle = EngineStateHelper::shaderPipeline(engineState, mesh.shaderPipelineIndex);

        glBindVertexArray(glMesh.vaoHandle);
        glBindProgramPipeline(shaderPipelineHandle);
        glUseProgramStages(
            shaderPipelineHandle,
            GL_VERTEX_SHADER_BIT,
            EngineStateHelper::shaderProgram(engineState, mesh.vertexShaderIndex));
        glUseProgramStages(
            shaderPipelineHandle,
            GL_FRAGMENT_SHADER_BIT,
            EngineStateHelper::shaderProgram(engineState, mesh.fragmentShaderIndex));

        auto texture = registry.try_get<Texture2d>(e);
        if (texture != nullptr)
        {
            auto texHandle = EngineStateHelper::texture2d(engineState, texture->index).handle;
            glBindTexture(GL_TEXTURE_2D, texHandle);
        }

        glDrawArrays((GLenum) mesh.renderMode, 0, (GLsizei) mesh.vertices.size());
    }
}
