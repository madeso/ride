#pragma once

#include "core/ints.h"

struct Color
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    constexpr Color()
        : r(0)
        , g(0)
        , b(0)
        , a(255)
    {
    }

    constexpr Color(u8 ar, u8 ag, u8 ab, u8 aa=255)
        : r(ar)
        , g(ag)
        , b(ab)
        , a(aa)
    {
    }

    static constexpr Color with_alpha(const Color& c, u8 aa=255)
    {
        return
        {
            c.r,
            c.g,
            c.b,
            aa
        };
    }

    constexpr static Color rgb(u8 r, u8 g, u8 b, u8 a)
    {
        return {r, g, b, a};
    }

    constexpr static u8 get(u32 c, int index)
    {
        const auto s = index == 0? c : c >> (index*8);
        return static_cast<u8>(0xFF & s);
    }

    constexpr static Color from_hex_with_alpha(u32 r)
    {
        return {get(r, 3), get(r, 2), get(r, 1), get(r, 0)};
    }

    constexpr static Color from_hex(u32 hex)
    {
        const auto r = get(hex, 2);
        const auto g = get(hex, 1);
        const auto b = get(hex, 0);
        return {r, g, b, 255};
    }

};
