#pragma once

#include <cstdint>

#include <components/key_code.h>

struct KeysState
{
    static constexpr size_t KEYS_COUNT = 256;
    uint8_t keyFlags[KEYS_COUNT];

    template<typename T>
    constexpr bool keyHasFlag(T key, uint8_t flag) const { return (keyFlags[static_cast<uint8_t>(key)] & flag) == flag; }
};