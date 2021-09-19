#pragma once


#include <cstdint>
#include <vector>
#include <memory>

#include "SDL.h"

#include "base/color.h"
#include "base/size.h"
#include "base/rect.h"

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

    // sub - screen coordinates
    // x/y - offset into image
    void draw_image(Image* image, const Rect& sub, int x, int y, Color color);
    int draw_text(Font* font, const char* text, int x, int y, Color color);
};
