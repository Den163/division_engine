#pragma once

#include "cstdint"

template<typename TValue>
inline TValue fastSelectArithmeticValue(bool isLeft, TValue left, TValue right)
{
    TValue castedIsLeft { static_cast<uint32_t>(isLeft) };
    TValue isRight { 1 - static_cast<uint32_t>(isLeft) };

    return castedIsLeft * left + isRight * right;
}