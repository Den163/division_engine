#pragma once

#include <cstdint>

struct WindowState
{
    int32_t screenSpaceWidth;
    int32_t screenSpaceHeight;
    int32_t frameBufferPixelsWidth;
    int32_t frameBufferPixelsHeight;
    bool shouldClose;
};