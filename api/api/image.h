#pragma once

#include <vector>
#include <cstdint>

#include "api/color.h"

struct Image
{
    std::vector<std::uint8_t> pixels;
    int width;
    int height;

    void setup(int width, int height);

    void set_color(int x, int y, const Color& c);
    Color get_color(int x, int y) const;
};

