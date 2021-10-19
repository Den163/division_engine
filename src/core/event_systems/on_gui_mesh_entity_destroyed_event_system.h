#pragma once

#include <entt/entt.hpp>

#include "../states/gl_shader_state.h"

namespace OnGuiMeshEntityDestroyedEventSystem
{
    void update(entt::registry& registry, const GlShaderState& shaderState);
}