#pragma once

#include <cstdint>

struct Color
{
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a;

    static Color rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
    static Color from_hex_with_alpha(std::uint32_t r);
    static Color from_hex(std::uint32_t r);
};
