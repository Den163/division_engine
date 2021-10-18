#pragma once

#include <cstdint>

#define INLINE_KEYBOARD_CHARS() \
    Q = 'Q', W = 'W', E = 'E', R = 'R', T = 'T', Y = 'Y', U = 'U', I = 'I', O = 'O', P = 'P', \
    A = 'A', S = 'S', D = 'D', F = 'F', G = 'G', H = 'H', J = 'J', K = 'K', L = 'L', \
    Z = 'Z', X = 'X', C = 'C', V = 'V', B = 'B', N = 'N', M = 'M',

#if defined(WIN32) || defined(_WIN32)
#include <Windows.h>
enum class KeyCode : uint8_t
{
    LEFT_ARROW = VK_LEFT,
    RIGHT_ARROW = VK_RIGHT,
    UP_ARROW = VK_UP,
    DOWN_ARROW = VK_DOWN,
    SPACE = VK_SPACE,
    ENTER = VK_RETURN,
    LEFT_MOUSE = VK_LBUTTON,
    RIGHT_MOUSE = VK_RBUTTON,

    INLINE_KEYBOARD_CHARS()
};
#endif