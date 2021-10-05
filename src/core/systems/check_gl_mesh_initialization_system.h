#pragma once

#include <entt/entt.hpp>
#include "../states/gl_shader_state.h"

namespace CheckGlMeshInitializationSystem
{
void update(entt::registry& ecsRegistry, GlShaderState& shaderState);
}