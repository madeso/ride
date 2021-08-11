#include "api/image.h"

#include <cassert>


void Image::setup(int width, int height)
{
    assert(width > 0 && height > 0);
    pixels.resize(width * height * 4);
    this->width = width;
    this->height = height;
}

void Image::set_color(int x, int y, const Color& c)
{
    const auto index = (x + y * width) * 4;
    pixels[index + 0] = c.r;
    pixels[index + 1] = c.g;
    pixels[index + 2] = c.b;
    pixels[index + 3] = c.a;
}

Color Image::get_color(int x, int y) const
{
    const auto index = (x + y * width) * 4;
    return Color::rgb(pixels[index + 0], pixels[index + 1], pixels[index + 2], pixels[index + 3]);
}
