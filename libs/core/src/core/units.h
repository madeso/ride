#pragma once

#include "core/number.h"

//virtual pixel (unscaled by user settings and dpi)
struct DeviceIndependentPixel_Tag {};
using Dp = Number<double, DeviceIndependentPixel_Tag>;

constexpr Dp operator"" _dp ( long double n )
{
    return Dp{static_cast<double>(n)};
}

constexpr Dp operator"" _dp ( unsigned long long n )
{
    return Dp{static_cast<double>(n)};
}

// actual pixel (pixel*scale)
struct Pixel_Tag {};
using Px = Number<int, Pixel_Tag>;

constexpr Px operator"" _px ( unsigned long long n )
{
    return Px{static_cast<int>(n)};
}
