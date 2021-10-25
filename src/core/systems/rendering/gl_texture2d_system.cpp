#include "gl_texture2d_system.h"

#include "../../components/gl_mesh.h"
#include "../../components/gl_texture.h"

void GlTextureSystem::update(EngineState& engineState)
{
    auto& registry = engineState.guiRegistry;
    for (auto&& [e, glMesh, texture] : registry.view<const GlMesh, const GlTexture>().each())
    {
        glBindVertexArray(glMesh.vaoHandle);
        glBindTexture(GL_TEXTURE_2D, texture.handle);
    }
}