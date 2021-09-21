#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "embed/types.h"

#include "api/units.h"

struct Color;
struct Ren;

struct FontImpl;

struct Font
{
    Font();
    ~Font();
    
    std::unique_ptr<FontImpl> m;

    static constexpr std::string_view default_font = "<default>";

    bool load_font(const std::string& filename, dip size);

    bool load_font(const embedded_binary& data, dip size);
    bool impl_load_font(std::unique_ptr<FontImpl> f, const unsigned char* data, dip size);

    void set_size(dip new_size);

    void set_tab_width(dip n);
    dip get_tab_width();

    pix unscaled_size = pix{0};

    dip get_width(const std::string& text);
    dip get_height();
};

int draw_text(Ren* ren, Font* font, const std::string& text, int x, int y, Color color);
