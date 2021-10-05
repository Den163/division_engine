#pragma once

template<typename T>
inline constexpr bool hasFlag(T mask, T flag) { return (mask & flag) == flag; }

template<typename T>
inline constexpr T selectScalar(bool predicate, T ifTrue, T ifFalse)
{
    return predicate * ifTrue + (1 - predicate) * ifFalse;
}