#include "api/font.h"

#include <cmath>

#include "stb_truetype.h"

#include "base/file.h"
#include "base/utf8.h"
#include "base/rect.h"

#include "api/image.h"
#include "api/renderer.h"

#include "font.ttf.h"

#define MAX_GLYPHSET 256

///////////////////////////////////////////////////////////////////////////////////////////////////
// Loaded FontData

struct LoadedFontData
{
    const unsigned char* data;
    std::vector<unsigned char> data_storage;  // ttf data
    stbtt_fontinfo stbfont;
    dip size = dip{0};
    dip height = dip{0};
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
        pixels.resize(width * height);

        /* load glyphs */
        const float s = stbtt_ScaleForMappingEmToPixels(&font->stbfont, 1) /
                        stbtt_ScaleForPixelHeight(&font->stbfont, 1);
        const int res = stbtt_BakeFontBitmap(font->data, 0, font->size.value * s, &pixels[0], width,
                                             height, idx * 256, 256, this->glyphs);

        if (res < 0)
        {
            return false;
        }

        /* adjust glyph yoffsets and xadvance */
        int ascent, descent, linegap;
        stbtt_GetFontVMetrics(&font->stbfont, &ascent, &descent, &linegap);
        const float scale = stbtt_ScaleForMappingEmToPixels(&font->stbfont, font->size.value);
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

        loaded = true;
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

    void mark_as_unloaded()
    {
        for(int i=0; i<MAX_GLYPHSET; i+=1)
        {
            sets[i].loaded = false;
        }
    }
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// Font

Font::Font() = default;

Font::~Font() = default;


bool Font::load_font(const embedded_binary& data, dip size)
{
    return impl_load_font(std::make_unique<FontImpl>(), reinterpret_cast<const unsigned char*>(data.data), size);
}

bool Font::load_font(const std::string& filename, dip size)
{
    if(filename == Font::default_font)
    {
        return load_font(FONT_TTF, size);
    }

    auto font = std::make_unique<FontImpl>();

    if (read_to_buffer(filename, &font->data.data_storage) == false)
    {
        return false;
    }

    const unsigned char* data = font->data.data_storage.data();
    return impl_load_font(std::move(font), data, size);
}

void set_size_for_font(FontImpl* font, dip size)
{
    int ascent, descent, linegap;
    stbtt_GetFontVMetrics(&font->data.stbfont, &ascent, &descent, &linegap);
    float scale = stbtt_ScaleForMappingEmToPixels(&font->data.stbfont, size.value);
    font->data.height = dip{static_cast<int>((ascent - descent + linegap) * scale + 0.5)};
}

bool Font::impl_load_font(std::unique_ptr<FontImpl> font, const unsigned char* data, dip size)
{
    /* init font */
    font->data.data = data;
    font->data.size = size;

    /* init stbfont */
    const auto ok = stbtt_InitFont(&font->data.stbfont, data, 0);
    if (!ok)
    {
        return false;
    }

    /* get height and scale */
    set_size_for_font(font.get(), size);

    /* make tab and newline glyphs invisible */
    stbtt_bakedchar* g = font->get_glyphset('\n')->glyphs;
    g['\t'].x1 = g['\t'].x0;
    g['\n'].x1 = g['\n'].x0;

    m = std::move(font);
    return true;
}

void Font::set_size(dip new_size)
{
    m->data.size = new_size;
    set_size_for_font(m.get(), new_size);
    m->mark_as_unloaded();
}

void Font::set_tab_width(dip n)
{
    GlyphSet* set = m->get_glyphset('\t');
    set->glyphs['\t'].xadvance = n.value;
}

dip Font::get_tab_width()
{
    GlyphSet* set = m->get_glyphset('\t');
    return dip{static_cast<int>(set->glyphs['\t'].xadvance)};
}

dip Font::get_width(const std::string& text)
{
    int x = 0;
    const auto codepoints = utf8_to_codepoints(text);
    for (const auto codepoint : codepoints)
    {
        GlyphSet* set = m->get_glyphset(codepoint);
        stbtt_bakedchar* g = &set->glyphs[codepoint & 0xff];
        x += g->xadvance;
    }
    return dip{x};
}

dip Font::get_height()
{
    return m->data.height;
}

int draw_text(Ren* ren, Font* font, const std::string& text, int x, int y, Color color)
{
    const auto codepoints = utf8_to_codepoints(text);
    for (const auto codepoint : codepoints)
    {
        GlyphSet* set = font->m->get_glyphset(codepoint);
        stbtt_bakedchar* g = &set->glyphs[codepoint & 0xff];
        auto rect = recti
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
