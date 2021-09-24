#include "base/color.h"

Color Color::rgb(u8 r, u8 g, u8 b, u8 a)
{
    return {r, g, b, a};
}

u8 get(u32 c, int index)
{
    const auto s = index == 0? c : c >> (index*8);
    return static_cast<u8>(0xFF & s);
}

Color Color::from_hex_with_alpha(u32 r)
{
    return {get(r, 3), get(r, 2), get(r, 1), get(r, 0)};
}

Color Color::from_hex(u32 hex)
{
    const auto r = get(hex, 2);
    const auto g = get(hex, 1);
    const auto b = get(hex, 0);
    return {r, g, b, 255};
}

Color chex(u32 r)
{
    return Color::from_hex(r);
}
