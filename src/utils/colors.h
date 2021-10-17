#pragma once

#include <glm/vec4.hpp>

namespace Colors
{
    const uint32_t R_MASK = 0xFF000000;
    const uint32_t G_MASK = 0x00FF0000;
    const uint32_t B_MASK = 0x0000FF00;
    const uint32_t A_MASK = 0x000000FF;

    const glm::vec4 red = { 1,0,0,1 };
    const glm::vec4 green = { 0,1,0,1 };
    const glm::vec4 blue = { 0,0,1,1 };
    const glm::vec4 cyan = { 0,1,1,1 };
    const glm::vec4 yellow = { 1,1,0,1 };
    const glm::vec4 black = { 0,0,0,1 };
    const glm::vec4 white = { 1,1,1,1 };

    glm::vec4 fromRgbaHex(uint32_t hexRgba)
    {
        return glm::vec4 {
            (double) (hexRgba & R_MASK) / R_MASK,
            (double) (hexRgba & G_MASK) / G_MASK,
            (double) (hexRgba & B_MASK) / B_MASK,
            (double) (hexRgba & A_MASK) / A_MASK
        };
    }

    uint32_t toRgbaHex(const glm::vec4& color)
    {
        auto r = (uint32_t) (((double) color.r) * R_MASK) & R_MASK;
        auto g = (uint32_t) (((double) color.g) * G_MASK) & G_MASK;
        auto b = (uint32_t) (((double) color.b) * B_MASK) & B_MASK;
        auto a = (uint32_t) (((double) color.a) * A_MASK) & A_MASK;

        return r + g + b + a;
    }
}