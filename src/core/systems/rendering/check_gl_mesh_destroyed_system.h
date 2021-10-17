#pragma once

#include <entt/entt.hpp>
#include "../../states/gl_shader_state.h"

namespace CheckGlMeshDestroyedSystem
{
    void update(entt::registry& ecsRegistry, const GlShaderState& shaderState);
}