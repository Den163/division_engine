#pragma once

#include <array>

#include <configs/shader_config.h>
#include <utils/engine_invariants.h>

namespace DefaultShadersConfig
{
    inline constexpr auto SHADERS_COUNT = 4;
    static inline constexpr std::array<ShaderConfig, SHADERS_COUNT> makeDefault()
    {
        std::array<ShaderConfig, SHADERS_COUNT> shaders {};
        shaders[EngineInvariants::STANDARD_VERTEX_SHADER_INDEX] =
            ShaderConfig { "shaders/default_gui.vert", ShaderType::Vertex };
        shaders[EngineInvariants::STANDARD_COLORED_FRAGMENT_SHADER_INDEX] =
            ShaderConfig { "shaders/default_gui_colored.frag", ShaderType::Fragment };
        shaders[EngineInvariants::STANDARD_TEXTURE_FRAGMENT_SHADER_INDEX] =
            ShaderConfig { "shaders/default_gui_texture.frag", ShaderType::Fragment };
        shaders[EngineInvariants::STANDARD_FONT_FRAGMENT_SHADER_INDEX] =
            ShaderConfig { "shaders/default_gui_font.frag", ShaderType::Fragment };

        return shaders;
    }

    inline constexpr auto defaultShaders = makeDefault();
};