#include "api/font.h"

#include <cmath>

#include "stb_truetype.h"

#include "api/image.h"
#include "api/file.h"
#include "api/utf8.h"
#include "api/rect.h"
#include "api/renderer.h"

#define MAX_GLYPHSET 256

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
// Font

bool Font::load_font(const std::string& filename, float size)
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

std::shared_ptr<Font> font_load(const std::string& file, float size)
{
    auto r = std::make_shared<Font>();
    if(r->load_font(file.c_str(), size))
    {
        return r;
    }
    else
    {
        return {};
    }
}

int draw_text(Ren* ren, Font* font, const char* text, int x, int y, Color color)
{
    const auto codepoints = utf8_to_codepoints(text);
    for (const auto codepoint : codepoints)
    {
        GlyphSet* set = font->m->get_glyphset(codepoint);
        stbtt_bakedchar* g = &set->glyphs[codepoint & 0xff];
        auto rect = Rect
        {
            g->x0,
            g->y0,
            g->x1 - g->x0,
            g->y1 - g->y0
        };
        ren->draw_image(&set->image, rect, x + g->xoff, y + g->yoff, color);
        x += g->xadvance;
    }

    return x;
}
