#pragma once

#include <cstdint>

namespace Memory
{
    template<typename TObject, typename TMember>
    inline constexpr size_t offsetOf(TMember TObject::*member)
    {
        constexpr TObject tempObj {};
        return size_t(&(tempObj.*member)) - size_t(&(tempObj));
    }
}