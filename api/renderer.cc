// #include <stdio.h>
#include <cassert>
#include <cmath>
#include <fstream>
#include <memory>

#include "stb_truetype.h"
#include "renderer.h"

#define MAX_GLYPHSET 256

///////////////////////////////////////////////////////////////////////////////////////////////////
// Util functions

std::vector<unsigned> utf8_to_codepoints(const char* text)
{
    std::vector<unsigned> dst;

    const char* p = text;

    unsigned res, n;
    while (*p)
    {
        switch (*p & 0xf0)
        {
        case 0xf0:
            res = *p & 0x07;
            n = 3;
            break;
        case 0xe0:
            res = *p & 0x0f;
            n = 2;
            break;
        case 0xd0:
        case 0xc0:
            res = *p & 0x1f;
            n = 1;
            break;
        default:
            res = *p;
            n = 0;
            break;
        }
        while (n--)
        {
            res = (res << 6) | (*(++p) & 0x3f);
        }
        dst.emplace_back(res);
        p = p + 1;
    }

    return dst;
}

bool read_to_buffer(const char* filename, std::vector<std::uint8_t>* buffer)
{
    std::ifstream infile(filename);

    if (infile.good() == false)
    {
        return false;
    }

    // get length of file
    infile.seekg(0, std::ios::end);
    size_t length = infile.tellg();
    infile.seekg(0, std::ios::beg);

    buffer->resize(length);
    infile.read(reinterpret_cast<char*>(buffer->data()), length);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Loaded FontData

struct LoadedFontData
{
    std::vector<unsigned char> data;  // ttf data
    stbtt_fontinfo stbfont;
    float size;
    int height;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// GlyphSet

struct GlyphSet
{
    bool loaded = false;
    Image image;
    stbtt_bakedchar glyphs[256];

    bool load_single_glyphset_or_fail(LoadedFontData* font, int idx, int width, int height)
    {
        std::vector<std::uint8_t> pixels;
        pixels.reserve(width * height);

        /* load glyphs */
        const float s = stbtt_ScaleForMappingEmToPixels(&font->stbfont, 1) /
                        stbtt_ScaleForPixelHeight(&font->stbfont, 1);
        const int res = stbtt_BakeFontBitmap(&font->data[0], 0, font->size * s, &pixels[0], width,
                                             height, idx * 256, 256, this->glyphs);

        if (res < 0)
        {
            return false;
        }

        /* adjust glyph yoffsets and xadvance */
        int ascent, descent, linegap;
        stbtt_GetFontVMetrics(&font->stbfont, &ascent, &descent, &linegap);
        const float scale = stbtt_ScaleForMappingEmToPixels(&font->stbfont, font->size);
        const int scaled_ascent = ascent * scale + 0.5;
        for (int i = 0; i < 256; i++)
        {
            this->glyphs[i].yoff += scaled_ascent;
            this->glyphs[i].xadvance = floor(this->glyphs[i].xadvance);
        }

        /* convert 8bit data to 32bit */
        this->image.setup(width, height);
        for (int y = 0; y < height; y += 1)
        {
            for (int x = 0; x < width; x += 1)
            {
                uint8_t n = pixels[y * width + x];
                this->image.set_color(x, y, Color::rgb(255, 255, 255, n));
            }
        }

        return true;
    }

    void load_glyphset(LoadedFontData* font, int idx)
    {
        int width = 128;
        int height = 128;

        while (load_single_glyphset_or_fail(font, idx, width, height) == false)
        {
            width *= 2;
            height *= 2;
        }
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// FontImpl

struct FontImpl
{
    LoadedFontData data;
    GlyphSet sets[MAX_GLYPHSET];

    GlyphSet* get_glyphset(int codepoint)
    {
        int idx = (codepoint >> 8) % MAX_GLYPHSET;
        if (sets[idx].loaded == false)
        {
            sets[idx].load_glyphset(&data, idx);
        }
        return &sets[idx];
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Color

Color Color::rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
{
    return {r, g, b, a};
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Image

void Image::setup(int width, int height)
{
    assert(width > 0 && height > 0);
    pixels.resize(width * height * 4);
    this->width = width;
    this->height = height;
}

void Image::set_color(int x, int y, const Color& c)
{
    const auto index = (x + y * width) * 4;
    pixels[index + 0] = c.r;
    pixels[index + 1] = c.g;
    pixels[index + 2] = c.b;
    pixels[index + 3] = c.a;
}

Color Image::get_color(int x, int y) const
{
    const auto index = (x + y * width) * 4;
    return Color::rgb(pixels[index + 0], pixels[index + 1], pixels[index + 2], pixels[index + 3]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Rect

int Rect::get_left() const
{
    return x;
}

int Rect::get_top() const
{
    return y;
}

int Rect::get_right() const
{
    return x + width;
}

int Rect::get_bottom() const
{
    return y + height;
}

bool Rect::overlap(Rect a, Rect b)
{
    return b.x + b.width >= a.x && b.x <= a.x + a.width && b.y + b.height >= a.y &&
           b.y <= a.y + a.height;
}

Rect Rect::intersect(Rect a, Rect b)
{
    int x1 = std::max(a.x, b.x);
    int y1 = std::max(a.y, b.y);
    int x2 = std::min(a.x + a.width, b.x + b.width);
    int y2 = std::min(a.y + a.height, b.y + b.height);
    return {x1, y1, std::max(0, x2 - x1), std::max(0, y2 - y1)};
}

Rect Rect::merge(Rect a, Rect b)
{
    int x1 = std::min(a.x, b.x);
    int y1 = std::min(a.y, b.y);
    int x2 = std::max(a.x + a.width, b.x + b.width);
    int y2 = std::max(a.y + a.height, b.y + b.height);
    return {x1, y1, x2 - x1, y2 - y1};
}

Rect Rect::from_size(const Size& s)
{
    return {0, 0, s.width, s.height};
}

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
// Font

bool Font::load_font(const char* filename, float size)
{
    auto font = std::make_unique<FontImpl>();

    /* init font */
    font->data.size = size;

    if (read_to_buffer(filename, &font->data.data) == false)
    {
        return false;
    }

    /* init stbfont */
    const auto ok = stbtt_InitFont(&font->data.stbfont, font->data.data.data(), 0);
    if (!ok)
    {
        return false;
    }

    /* get height and scale */
    int ascent, descent, linegap;
    stbtt_GetFontVMetrics(&font->data.stbfont, &ascent, &descent, &linegap);
    float scale = stbtt_ScaleForMappingEmToPixels(&font->data.stbfont, size);
    font->data.height = (ascent - descent + linegap) * scale + 0.5;

    /* make tab and newline glyphs invisible */
    stbtt_bakedchar* g = font->get_glyphset('\n')->glyphs;
    g['\t'].x1 = g['\t'].x0;
    g['\n'].x1 = g['\n'].x0;

    m = std::move(font);
    return true;
}

void Font::set_tab_width(int n)
{
    GlyphSet* set = m->get_glyphset('\t');
    set->glyphs['\t'].xadvance = n;
}

int Font::get_tab_width()
{
    GlyphSet* set = m->get_glyphset('\t');
    return set->glyphs['\t'].xadvance;
}

int Font::get_width(const char* text)
{
    int x = 0;
    const auto codepoints = utf8_to_codepoints(text);
    for (const auto codepoint : codepoints)
    {
        GlyphSet* set = m->get_glyphset(codepoint);
        stbtt_bakedchar* g = &set->glyphs[codepoint & 0xff];
        x += g->xadvance;
    }
    return x;
}

int Font::get_height()
{
    return m->data.height;
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

void Ren::draw_image(Image* image, Rect* sub, int x, int y, Color color)
{
    if (color.a == 0)
    {
        return;
    }

    /* clip */
    int n;
    if ((n = clip.left - x) > 0)
    {
        sub->width -= n;
        sub->x += n;
        x += n;
    }
    if ((n = clip.top - y) > 0)
    {
        sub->height -= n;
        sub->y += n;
        y += n;
    }
    if ((n = x + sub->width - clip.right) > 0)
    {
        sub->width -= n;
    }
    if ((n = y + sub->height - clip.bottom) > 0)
    {
        sub->height -= n;
    }

    if (sub->width <= 0 || sub->height <= 0)
    {
        return;
    }

    /* draw */
    SDL_Surface* surf = SDL_GetWindowSurface(window);

    for (int yy = 0; yy < sub->height; yy++)
    {
        for (int xx = 0; xx < sub->width; xx++)
        {
            const auto dest_x = sub->x + xx;
            const auto dest_y = sub->y + yy;
            const auto image_color = image->get_color(x + xx, y + yy);
            const auto new_color =
                blend_pixel2(get_pixel_on_surface(surf, dest_x, dest_y), image_color, color);
            set_pixel_on_surface(surf, dest_x, dest_y, new_color);
        }
    }
}

int Ren::draw_text(Font* font, const char* text, int x, int y, Color color)
{
    const auto codepoints = utf8_to_codepoints(text);
    for (const auto codepoint : codepoints)
    {
        GlyphSet* set = font->m->get_glyphset(codepoint);
        stbtt_bakedchar* g = &set->glyphs[codepoint & 0xff];
        Rect rect;
        rect.x = g->x0;
        rect.y = g->y0;
        rect.width = g->x1 - g->x0;
        rect.height = g->y1 - g->y0;
        draw_image(&set->image, &rect, x + g->xoff, y + g->yoff, color);
        x += g->xadvance;
    }

    return x;
}
