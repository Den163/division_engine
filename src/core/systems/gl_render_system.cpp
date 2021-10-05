#include <entt/entity/registry.hpp>
#include "gl_render_system.h"

#include "../../utils/debug_utils.h"
#include "../components/mesh.h"
#include "../components/gl_mesh.h"

void GlRenderSystem::init(GlShaderState& shaderState, const WindowConfig& windowConfig)
{
//    glViewport(0, 0, windowCfg.width, windowCfg.height);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugUtils::glRendererMessageCallback, nullptr);

    glCreateVertexArrays(GlShaderState::VERTEX_ARRAYS_COUNT, shaderState.vertexArrayHandles);
}

void GlRenderSystem::update(entt::registry& ecsRegistry, GlShaderState& shaderState, const RendererConfig& rendererConfig)
{
    auto& vaoHandle = shaderState.vertexArrayHandles[0];

    glClearBufferfv(GL_COLOR, 0, reinterpret_cast<const GLfloat*>(&rendererConfig.backgroundColor));

    for (auto&& [e, mesh, glMesh] : ecsRegistry.view<Mesh, GlMesh>().each())
    {
        const auto& vboHandle = glMesh.vboHandle;
        const auto& vertices = mesh.vertices;
        const auto* verticesPtr = vertices.data();
        const auto verticesCount = vertices.size();
        const auto verticesBytes = static_cast<GLsizeiptr>(verticesCount * sizeof(Mesh::Vertex));

        glBindVertexArray(vaoHandle);

        GLint bufferSize;
        glGetNamedBufferParameteriv(vboHandle, GL_BUFFER_SIZE, &bufferSize);

        if (bufferSize == verticesBytes)
        {
            glNamedBufferSubData(vboHandle, 0, verticesBytes, verticesPtr);
        }
        else
        {
            glNamedBufferStorage(vboHandle, verticesBytes, verticesPtr, GL_NONE);
        }

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(verticesCount));
    }
}