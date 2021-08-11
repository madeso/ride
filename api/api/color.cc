#include "api/color.h"

Color Color::rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
{
    return {r, g, b, a};
}

std::uint8_t get(std::uint32_t c, int index)
{
    const auto s = index == 0? c : c << (index*8);
    return static_cast<std::uint8_t>(0xFF & s);
}

Color Color::from_hex_with_alpha(std::uint32_t r)
{
    return {get(r, 3), get(r, 2), get(r, 1), get(r, 0)};
}

Color Color::from_hex(std::uint32_t r)
{
    return {get(r, 2), get(r, 1), get(r, 0), 255};
}
