#pragma once

#include "base/number.h"

//virtual pixel
struct pixels_tag {};
using pix = number<double, pixels_tag>;

constexpr pix operator"" _px ( long double n )
{
    return pix{static_cast<double>(n)};
}

constexpr pix operator"" _px ( unsigned long long n )
{
    return pix{static_cast<double>(n)};
}

// actual pixel (pixel*scale)
struct device_pixels_tag {};
using dip = number<int, device_pixels_tag>;
