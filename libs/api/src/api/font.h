#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "embed/types.h"

struct Color;
struct Ren;

struct FontImpl;

struct Font
{
    Font();
    ~Font();
    
    std::unique_ptr<FontImpl> m;

    static constexpr std::string_view default_font = "<default>";

    bool load_font(const std::string& filename, float size);

    bool load_font(const embedded_binary& data, float size);
    bool impl_load_font(std::unique_ptr<FontImpl> f, const unsigned char* data, float size);

    void set_size(float new_size);

    void set_tab_width(int n);
    int get_tab_width();

    double unscaled_size;

    int get_width(const std::string& text);
    int get_height();
};

int draw_text(Ren* ren, Font* font, const std::string& text, int x, int y, Color color);
