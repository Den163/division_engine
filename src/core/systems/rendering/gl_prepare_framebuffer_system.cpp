#include "gl_prepare_framebuffer_system.h"

void GlPrepareFramebufferSystem::update(EngineState& engineState)
{
    const auto& windowState = engineState.windowState;
    const auto& rendererState = engineState.rendererState;

    glViewport(0, 0, windowState.width, windowState.height);
    glClearBufferfv(GL_COLOR, 0, reinterpret_cast<const GLfloat*>(&rendererState.backgroundColor));
}
