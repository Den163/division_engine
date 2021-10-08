#pragma once

#include "../../states/gl_shader_state.h"
#include "../../configs/renderer_config.h"
#include "../../configs/window_config.h"
#include "../../states/camera_state.h"
#include "../../states/window_state.h"
#include "../../states/renderer_state.h"

namespace GlRenderSystem
{
void init(GlShaderState& shaderState, CameraState& cameraState, const WindowState& windowState);
void update(
    entt::registry& ecsRegistry, GlShaderState& shaderState, const RendererState& rendererState,
    const CameraState& cameraState, const WindowState& windowState);
}