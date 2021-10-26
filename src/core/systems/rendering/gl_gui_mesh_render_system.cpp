#include "gl_gui_mesh_render_system.h"

#include "../../components/gl_mesh.h"
#include "../../components/gui_mesh.h"

void GlRenderGuiMeshSystem::update(EngineState& engineState)
{
    auto& registry = engineState.guiRegistry;

    for (auto&& [e, glMesh, mesh] : registry.view<const GlMesh, const GuiMesh>().each())
    {
        const auto shaderPipelineHandle = mesh.shaderPipelineHandle;

        glBindVertexArray(glMesh.vaoHandle);
        glBindProgramPipeline(shaderPipelineHandle);
        glUseProgramStages(
            shaderPipelineHandle,
            GL_VERTEX_SHADER_BIT,
            mesh.vertexShaderHandle);
        glUseProgramStages(
            shaderPipelineHandle,
            GL_FRAGMENT_SHADER_BIT,
            mesh.fragmentShaderHandle);

        glDrawArrays((GLenum) mesh.renderMode, 0, (GLsizei) mesh.vertices.size());
    }
}
