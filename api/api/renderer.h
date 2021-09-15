#pragma once


#include <cstdint>
#include <vector>
#include <memory>

#include "SDL.h"

#include "api/color.h"

#include "api/color.h"
#include "api/size.h"
#include "api/rect.h"

struct Font;
struct Image;

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
