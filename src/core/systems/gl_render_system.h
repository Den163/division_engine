#pragma once

#include "../components/gl_shader_state.h"
#include "../components/renderer_config.h"
#include "../components/window_config.h"

namespace GlRenderSystem
{
void init(GlShaderState& shaderState, const WindowConfig& windowConfig);
void update(GlShaderState& shaderProgram, const RendererConfig& rendererConfig);
}