#pragma once

#include "base/ints.h"

struct Color
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    static Color rgb(u8 r, u8 g, u8 b, u8 a);
    static Color from_hex_with_alpha(u32 r);
    static Color from_hex(u32 r);
};

Color chex(u32 r);
