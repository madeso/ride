#pragma once

#include <string>

#include "base/units.h"

static constexpr std::string_view default_font = "<default>";

struct Font
{
    Font() = default;
    virtual ~Font() = default;

    virtual bool load_font(const std::string& filename, Px size) = 0;
    Dp unscaled_size = Dp{0};

    virtual Px get_width(const std::string& text) = 0;
    virtual Px get_height() const = 0;
};

