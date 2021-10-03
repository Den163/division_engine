#include "gl_render_system.h"

#include "../../utils/debug_utils.h"

void GlRenderSystem::init(GlShaderState& shaderState, const WindowConfig& windowConfig)
{
//    glViewport(0, 0, windowCfg.width, windowCfg.height);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugUtils::glRendererMessageCallback, nullptr);

    glCreateVertexArrays(GlShaderState::VERTEX_ARRAYS_COUNT, shaderState.vertexArrayHandles);
    glCreateBuffers(GlShaderState::VERTEX_BUFFERS_COUNT, shaderState.vertexBufferHandles);
    glNamedBufferStorage(
        shaderState.vertexBufferHandles[GlShaderState::POSITIONS_VBO_INDEX],
        shaderState.vertexBuffer.size() * sizeof(float),
        shaderState.vertexBuffer.data(),
        0
    );

    glBindVertexArray(shaderState.vertexArrayHandles[GlShaderState::TRIANGLES_ARRAY_INDEX]);
    glBindBuffer(GL_ARRAY_BUFFER, shaderState.vertexBufferHandles[GlShaderState::POSITIONS_VBO_INDEX]);
    glVertexAttribPointer(GlShaderState::POSITIONS_VBO_INDEX, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(GlShaderState::POSITIONS_VBO_INDEX);
}

void GlRenderSystem::update(GlShaderState& shaderProgram, const RendererConfig& rendererConfig)
{
    auto& vaoHandle = shaderProgram.vertexArrayHandles[0];

    glClearBufferfv(GL_COLOR, 0, reinterpret_cast<const GLfloat*>(&rendererConfig.backgroundColor));

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, shaderProgram.vertexBuffer.size());
}