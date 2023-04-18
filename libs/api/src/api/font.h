#pragma once

#include "core/units.h"

static constexpr std::string_view default_font = "<default>";

struct Platform;
struct Color;
struct Renderer;

struct Font
{
    Font() = default;
    virtual ~Font() = default;

    virtual bool load_font(const std::string& filename, Px size, Platform* app) = 0;
    Dp unscaled_size = Dp{0};

    virtual Px get_width(const std::string& text) = 0;
    virtual Px get_height() const = 0;

    virtual Px draw(Renderer* rend, const std::string& text, Px x, Px y, Color color) = 0;
};

