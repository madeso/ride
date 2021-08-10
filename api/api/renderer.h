#pragma once

#include "api/color.h"

#include <cstdint>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>

struct FontImpl;

struct Font
{
    std::unique_ptr<FontImpl> m;

    bool load_font(const char* filename, float size);

    void set_tab_width(int n);
    int get_tab_width();

    int get_width(const char* text);
    int get_height();
};

struct Image
{
    std::vector<std::uint8_t> pixels;
    int width;
    int height;

    void setup(int width, int height);

    void set_color(int x, int y, const Color& c);
    Color get_color(int x, int y) const;
};

struct Size
{
    int width;
    int height;
};

struct Rect
{
    int x;
    int y;
    int width;
    int height;

    int get_left() const;
    int get_top() const;
    int get_right() const;
    int get_bottom() const;

    static bool overlap(Rect a, Rect b);
    static Rect intersect(Rect a, Rect b);
    static Rect merge(Rect a, Rect b);

    static Rect from_size(const Size& s);
};

struct Clip
{
    int left;
    int top;
    int right;
    int bottom;

    int get_left() const;
    int get_top() const;
    int get_right() const;
    int get_bottom() const;
};

struct Ren
{
    SDL_Window* window;
    Clip clip;
    bool initial_frame = true;

    void init(SDL_Window* win);
    void update_rects(std::vector<Rect>& rects);
    void set_clip_rect(Rect rect);
    Size get_size();

    void draw_rect(Rect rect, Color color);
    void draw_image(Image* image, Rect* sub, int x, int y, Color color);
    int draw_text(Font* font, const char* text, int x, int y, Color color);
};
