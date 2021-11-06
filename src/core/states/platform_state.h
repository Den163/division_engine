#pragma once

#if defined(WIN32) || defined(_WIN32)
    #include "win32_state.h"
#endif

struct PlatformState
{
#if defined(WIN32) || defined(_WIN32)
    Win32State win32;
#endif
};
