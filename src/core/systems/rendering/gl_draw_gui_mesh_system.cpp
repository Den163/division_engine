#include "gl_draw_gui_mesh_system.h"
#include "../../components/gl_mesh.h"
#include "../../components/gui_material.h"
#include "../../components/gl_draw_indirect_command.h"
#include "../../components/gl_texture.h"

void GlDrawGuiMeshSystem::update(EngineState& engineState)
{
    for (auto&& [e, glMesh, material] :
        engineState.guiRegistry.view<GlMesh, GuiMaterial>().each())
    {
        const auto shaderPipelineHandle = material.shaderPipelineHandle;
        glBindVertexArray(glMesh.vaoHandle);
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
        glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, nullptr, glMesh.primitivesCount, sizeof(GlDrawArraysIndirectCommand));
    }
}
