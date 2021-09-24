#include "api/renderer.h"

// #include <stdio.h>
#include <cassert>
#include <cmath>
#include <memory>

#include "base/c.h"

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

namespace
{
    constexpr u8 c_int_to_u8(int i)
    {
        return static_cast<u8>(i);
    }

    constexpr u8 blend(u8 srcc, u8 a, u8 dstc)
    {
        const auto ia = 0xff - a;
        const auto s = srcc * a;
        const auto d = dstc * ia;
        return c_int_to_u8((s + d) >> 8);
    };

    constexpr u8 blend2(u8 srcc, u8 cc, u8 a, u8 dstc)
    {
        const auto ia = 0xff - a;
        const auto s = srcc * cc * a;
        const auto d = dstc * ia;
        return c_int_to_u8((s >> 16) + (d >> 8));
    };
}

Color blend_pixel(Color dst, Color src)
{
    dst.r = blend(src.r, src.a, dst.r);
    dst.g = blend(src.g, src.a, dst.g);
    dst.b = blend(src.b, src.a, dst.b);
    return dst;
}

Color blend_pixel2(Color dst, Color src, Color color)
{
    src.a = (src.a * color.a) >> 8;
    
    dst.r = blend2(src.r, color.r, src.a, dst.r);
    dst.g = blend2(src.g, color.g, src.a, dst.g);
    dst.b = blend2(src.b, color.b, src.a, dst.b);
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

void Ren::update_rects(std::vector<recti>& rects)
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
        SDL_UpdateWindowSurfaceRects(window, sdl_rects.data(), C(sdl_rects.size()));
    }

    if (initial_frame)
    {
        SDL_ShowWindow(window);
        initial_frame = false;
    }
}

void Ren::set_clip_rect(recti rect)
{
    clip.left = rect.x;
    clip.top = rect.y;
    clip.right = rect.x + rect.width;
    clip.bottom = rect.y + rect.height;
}

sizei Ren::get_size()
{
    SDL_Surface* surf = SDL_GetWindowSurface(window);
    return {surf->w, surf->h};
}

u32* get_pixels_from_sdl_surface(SDL_Surface* surface)
{
    return static_cast<u32*>(surface->pixels);
}

void set_pixel_on_surface(SDL_Surface* surface, int x, int y, const Color& c)
{
    assert(surface);
    const auto inside = x >= 0 && x < surface->w && y >= 0 && y < surface->h;
    if(!inside)
    {
        return;
    }

    auto* pixels = get_pixels_from_sdl_surface(surface);
    pixels[y * surface->w + x] = SDL_MapRGB(surface->format, c.r, c.g, c.b);
}

Color get_pixel_on_surface(SDL_Surface* surface, int x, int y)
{
    assert(surface);
    const auto inside = x >= 0 && x < surface->w && y >= 0 && y < surface->h;
    if(!inside)
    {
        return {255,255,255,255};
    }

    auto* pixels = get_pixels_from_sdl_surface(surface);
    const auto pixel = pixels[y * surface->w + x];

    Color c;
    SDL_GetRGB(pixel, surface->format, &c.r, &c.g, &c.b);

    return c;
}

void Ren::draw_rect(recti rect, Color color)
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

void Ren::draw_image(Image* image, const recti& asub, int x, int y, Color color)
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
            const auto dest_x = x + xx;
            const auto dest_y = y + yy;
            const auto image_color = image->get_color(sub.x + xx, sub.y + yy);
            const auto new_color = blend_pixel2
            (
                get_pixel_on_surface(surf, dest_x, dest_y),
                image_color,
                color
            );
            set_pixel_on_surface(surf, dest_x, dest_y, new_color);
        }
    }
}

int Ren::draw_text(Font* font, const std::string& text, int x, int y, Color color)
{
    return ::draw_text(this, font, text, x, y, color);
}
