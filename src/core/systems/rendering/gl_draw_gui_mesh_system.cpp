#include "gl_draw_gui_mesh_system.h"
#include "../../components/gl_mesh.h"
#include "../../components/gui_material.h"
#include "../../components/gl_draw_indirect_command.h"
#include "../../components/gl_texture.h"
#include "../../components/gui_mesh.h"

void GlDrawGuiMeshSystem::update(EngineState& engineState)
{
    for (auto&& [e, glMesh, guiMesh, material] :
        engineState.guiRegistry.view<const GlMesh, const GuiMesh, const GuiMaterial>().each())
    {
        const auto shaderPipelineHandle = material.shaderPipelineHandle;
        glBindVertexArray(glMesh.vaoHandle);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, glMesh.indirectBufferHandle);
        glBindProgramPipeline(shaderPipelineHandle);
        glUseProgramStages(
            shaderPipelineHandle,
            GL_VERTEX_SHADER_BIT,
            material.vertexShaderProgramHandle);
        glUseProgramStages(
            shaderPipelineHandle,
            GL_FRAGMENT_SHADER_BIT,
            material.fragmentShaderProgramHandle);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto* texturePtr = engineState.guiRegistry.try_get<GlTexture>(e);
        if (texturePtr != nullptr)
        {
            glBindTexture(GL_TEXTURE_2D, texturePtr->handle);
        }
        glMultiDrawArraysIndirect(
            glMesh.renderMode, nullptr, guiMesh.primitivesCount, sizeof(GlDrawArraysIndirectCommand));
    }
}
