#pragma once

#include "base/number.h"

// todo(Gustav): swap the name for theese...
// pixel should be the device pixels and 
// dip (or dp) should be the Device Independent Pixel

//virtual pixel
struct pixels_tag {};
using pix = Number<double, pixels_tag>;

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
using dip = Number<int, device_pixels_tag>;

constexpr dip operator"" _dp ( unsigned long long n )
{
    return dip{static_cast<int>(n)};
}
