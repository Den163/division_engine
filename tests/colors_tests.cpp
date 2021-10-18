#include <gtest/gtest.h>
#include <map>
#include "../src/utils/color.h"
#include "../src/utils/debug_utils.h"

std::map<uint32_t, glm::vec4> testCases {
    { 0x00000000, { 0,0,0,0 } },
    { 0xFF000000, { 1,0,0,0 } },
    { 0x00FF0000, { 0,1,0,0 } },
    { 0x0000FF00, { 0,0,1,0 } },
    { 0x000000FF, { 0,0,0,1 } },
    { 0xFFFFFFFF, { 1,1,1,1 } },
    { 0xFF00FFFF, { 1,0,1,1 } },
    { 0xFF0000FF, { 1,0,0,1 } },
    { 0x7F000000, { .5f,0.f,0.f,0.f } },
    { 0x7F7F7F7F, { .5f,.5f,.5f,.5f } },
};

TEST(COLORS_TESTS, fromRgbaHex_returns_color_as_expected)
{
    const auto ERROR_TOLERANCE = 0.01f;
    for (auto [hexColor, expected] : testCases)
    {
        auto result = Color::fromRgbaHex(hexColor);

        EXPECT_NEAR(expected.r, result.r, ERROR_TOLERANCE);
        EXPECT_NEAR(expected.g, result.g, ERROR_TOLERANCE);
        EXPECT_NEAR(expected.b, result.b, ERROR_TOLERANCE);
        EXPECT_NEAR(expected.a, result.a, ERROR_TOLERANCE);
    }
}

TEST(COLORS_TESTS, toRgbaHex_returns_as_expected)
{
    for (auto [expected, vectorColor] : testCases)
    {
        auto result = Color::toRgbaHex(vectorColor);
        EXPECT_EQ(expected, result)
                        << "Expected color vector: " << vectorColor << std::endl
                        << "Actual color vector: " << Color::fromRgbaHex(result) << std::endl
            << "Expected (hex): 0x" << std::hex << expected << std::endl
            << "Actual (hex): 0x" << std::hex << result << std::endl;
    }
}