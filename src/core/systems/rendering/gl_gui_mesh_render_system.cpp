#include "gl_gui_mesh_render_system.h"

#include "../../components/gl_mesh.h"
#include "../../components/gui_mesh.h"
#include "../../components/gl_texture.h"

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
            mesh.vertexShaderProgramHandle);
        glUseProgramStages(
            shaderPipelineHandle,
            GL_FRAGMENT_SHADER_BIT,
            mesh.fragmentShaderProgramHandle);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto* texture = registry.try_get<GlTexture>(e);
        if (texture != nullptr)
        {
            glBindTexture(GL_TEXTURE_2D, texture->handle);
        }
        glDrawArrays((GLenum) mesh.renderMode, 0, (GLsizei) mesh.vertices.size());
    }
}
