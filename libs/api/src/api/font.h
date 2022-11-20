#pragma once

#include <string>

#include "base/units.h"

static constexpr std::string_view default_font = "<default>";

struct Font
{
    Font() = default;
    virtual ~Font() = default;

    virtual bool load_font(const std::string& filename, dip size) = 0;
    pix unscaled_size = pix{0};

    virtual dip get_width(const std::string& text) = 0;
    virtual dip get_height() const = 0;
};

