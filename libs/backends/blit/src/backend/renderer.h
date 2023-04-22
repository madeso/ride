#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <memory>

#include "backend/dependency_sdl.h"

#include "core/color.h"
#include "core/size.h"
#include "core/rect.h"

namespace ride::api
{
    struct Image;
    struct Font;
}

namespace ride::backends::blit
{

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
    void update_rects(std::vector<Recti>& rects);
    void set_clip_rect(Recti rect);
    Sizei get_size();

    void draw_rect(Recti rect, Color color);

    // sub - screen coordinates
    // x/y - offset into image
    void draw_image(api::Image* image, const Recti& sub, int x, int y, Color color);
    int draw_text(api::Font* font, const std::string& text, int x, int y, Color color);
};

}
