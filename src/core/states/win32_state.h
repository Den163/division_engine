#pragma once

#if defined(WIN32) || defined(_WIN32)
#include <Windows.h>

struct Win32State
{
    HWND windowHandle;
};

#endif