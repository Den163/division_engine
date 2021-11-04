#include "gl_prepare_framebuffer_system.h"

void GlPrepareFramebufferSystem::update(EngineState& engineState)
{
    const auto& windowState = engineState.window;
    const auto& rendererState = engineState.renderer;

    glClearBufferfv(GL_COLOR, 0, reinterpret_cast<const GLfloat*>(&rendererState.backgroundColor));
}
