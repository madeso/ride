#pragma once

#include <memory>
#include <string>

struct Color;
struct Ren;

struct FontImpl;

struct Font
{
    std::unique_ptr<FontImpl> m;

    bool load_font(const std::string& filename, float size);

    void set_tab_width(int n);
    int get_tab_width();

    int get_width(const char* text);
    int get_height();
};

std::shared_ptr<Font> font_load(const std::string& file, float size);

int draw_text(Ren* ren, Font* font, const char* text, int x, int y, Color color);
