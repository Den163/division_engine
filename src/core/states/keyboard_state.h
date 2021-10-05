#pragma once

#include <cstdint>

struct KeyboardState
{
    static constexpr size_t KEYS_COUNT = 256;
    uint8_t keyFlags[KEYS_COUNT];

    inline bool keyHasFlag(uint8_t key, uint8_t flag) const { return (keyFlags[key] & flag) == flag; }
};