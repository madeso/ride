#include "api/color.h"

Color Color::rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
{
    return {r, g, b, a};
}
