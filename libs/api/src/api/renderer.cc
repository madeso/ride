#include "api/renderer.h"

// #include <stdio.h>
#include <cassert>
#include <cmath>
#include <memory>


#include "api/font.h"
#include "api/image.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Clip

int Clip::get_left() const
{
    return left;
}

int Clip::get_top() const
{
    return top;
}

int Clip::get_right() const
{
    return right;
}

int Clip::get_bottom() const
{
    return bottom;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Blending

Color blend_pixel(Color dst, Color src)
{
    int ia = 0xff - src.a;
    dst.r = ((src.r * src.a) + (dst.r * ia)) >> 8;
    dst.g = ((src.g * src.a) + (dst.g * ia)) >> 8;
    dst.b = ((src.b * src.a) + (dst.b * ia)) >> 8;
    return dst;
}

Color blend_pixel2(Color dst, Color src, Color color)
{
    src.a = (src.a * color.a) >> 8;
    int ia = 0xff - src.a;
    dst.r = ((src.r * color.r * src.a) >> 16) + ((dst.r * ia) >> 8);
    dst.g = ((src.g * color.g * src.a) >> 16) + ((dst.g * ia) >> 8);
    dst.b = ((src.b * color.b * src.a) >> 16) + ((dst.b * ia) >> 8);
    return dst;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Ren

void Ren::init(SDL_Window* win)
{
    assert(win);
    window = win;
    SDL_Surface* surf = SDL_GetWindowSurface(window);
    set_clip_rect({0, 0, surf->w, surf->h});
}

void Ren::update_rects(std::vector<Rect>& rects)
{
    if (rects.empty() == false)
    {
        std::vector<SDL_Rect> sdl_rects;
        sdl_rects.reserve(rects.size());
        for (const auto& r : rects)
        {
            SDL_Rect rr;
            rr.x = r.x;
            rr.y = r.y;
            rr.w = r.width;
            rr.h = r.height;
            sdl_rects.emplace_back(rr);
        }
        SDL_UpdateWindowSurfaceRects(window, sdl_rects.data(), sdl_rects.size());
    }

    if (initial_frame)
    {
        SDL_ShowWindow(window);
        initial_frame = false;
    }
}

void Ren::set_clip_rect(Rect rect)
{
    clip.left = rect.x;
    clip.top = rect.y;
    clip.right = rect.x + rect.width;
    clip.bottom = rect.y + rect.height;
}

Size Ren::get_size()
{
    SDL_Surface* surf = SDL_GetWindowSurface(window);
    return {surf->w, surf->h};
}

void set_pixel_on_surface(SDL_Surface* surface, int x, int y, const Color& c)
{
    auto* pixels = (std::uint32_t*)surface->pixels;
    pixels[y * surface->w + x] = SDL_MapRGB(surface->format, c.r, c.g, c.b);
}

Color get_pixel_on_surface(SDL_Surface* surface, int x, int y)
{
    auto* pixels = (std::uint32_t*)surface->pixels;
    const auto pixel = pixels[y * surface->w + x];

    Color c;
    SDL_GetRGB(pixel, surface->format, &c.r, &c.g, &c.b);

    return c;
}

void Ren::draw_rect(Rect rect, Color color)
{
    if (color.a == 0)
    {
        return;
    }

    const int left = std::max(rect.get_left(), clip.get_left());
    const int top = std::max(rect.get_top(), clip.get_top());
    const int right = std::min(rect.get_right(), clip.get_right());
    const int bottom = std::min(rect.get_bottom(), clip.get_bottom());

    SDL_Surface* surf = SDL_GetWindowSurface(window);

    SDL_LockSurface(surf);

    for (int y = top; y < bottom; y++)
    {
        for (int x = left; x < right; x++)
        {
            const auto c =
                (color.a == 0xff) ? color : blend_pixel(get_pixel_on_surface(surf, x, y), color);
            set_pixel_on_surface(surf, x, y, c);
        }
    }

    SDL_UnlockSurface(surf);
}

void Ren::draw_image(Image* image, const Rect& asub, int x, int y, Color color)
{
    if (color.a == 0)
    {
        return;
    }

    auto sub = asub;

    /* clip */
    int n;
    
    n = clip.left - x;
    if (n > 0)
    {
        sub.width -= n;
        sub.x += n;
        x += n;
    }

    n = clip.top - y;
    if (n > 0)
    {
        sub.height -= n;
        sub.y += n;
        y += n;
    }
    
    n = x + sub.width - clip.right;
    if (n > 0)
    {
        sub.width -= n;
    }

    n = y + sub.height - clip.bottom;
    if (n > 0)
    {
        sub.height -= n;
    }

    if (sub.width <= 0 || sub.height <= 0)
    {
        return;
    }

    /* draw */
    SDL_Surface* surf = SDL_GetWindowSurface(window);

    for (int yy = 0; yy < sub.height; yy++)
    {
        for (int xx = 0; xx < sub.width; xx++)
        {
            const auto dest_x = sub.x + xx;
            const auto dest_y = sub.y + yy;
            const auto image_color = image->get_color(x + xx, y + yy);
            const auto new_color =
                blend_pixel2(get_pixel_on_surface(surf, dest_x, dest_y), image_color, color);
            set_pixel_on_surface(surf, dest_x, dest_y, new_color);
        }
    }
}

int Ren::draw_text(Font* font, const char* text, int x, int y, Color color)
{
    return ::draw_text(this, font, text, x, y, color);
}
