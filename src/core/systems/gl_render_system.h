#pragma once

#include "../states/gl_shader_state.h"
#include "../configs/renderer_config.h"
#include "../configs/window_config.h"

namespace GlRenderSystem
{
void init(GlShaderState& shaderState, const WindowConfig& windowConfig);
void update(entt::registry& ecsRegistry, GlShaderState& shaderState, const RendererConfig& rendererConfig);
}