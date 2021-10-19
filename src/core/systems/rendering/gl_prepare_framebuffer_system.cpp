#include "gl_prepare_framebuffer_system.h"

#include <glad/gl.h>

void GlPrepareFramebufferSystem::update(const RendererState& rendererState, const WindowState& windowState)
{
    glViewport(0, 0, windowState.width, windowState.height);
    glClearBufferfv(GL_COLOR, 0, reinterpret_cast<const GLfloat*>(&rendererState.backgroundColor));
}
