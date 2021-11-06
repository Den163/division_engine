#pragma once

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdeprecated-volatile"
#if defined(WIN32) || defined(_WIN32)
    #include <windows.h>
#endif
    #include <glm/glm.hpp>
    #pragma clang diagnostic pop
#endif
