#pragma once

struct TestType
{
    int32_t x;
    int32_t y;

    bool operator==(const TestType& other) const { return x == other.x && y == other.y; }
};