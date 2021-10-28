#pragma once

#include "shader_config.h"
#include <array>

namespace DefaultShadersConfig
{
    inline constexpr std::array<ShaderConfig, 4> defaultShaders
    {
        ShaderConfig { "shaders/default_gui.vert", ShaderType::Vertex },
        ShaderConfig { "shaders/default_gui_colored.frag", ShaderType::Fragment },
        ShaderConfig { "shaders/default_gui_texture.frag", ShaderType::Fragment },
        ShaderConfig { "shaders/default_gui_font.frag", ShaderType::Fragment },
    };
};