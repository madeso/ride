#include "apiact/font.h"

#include <cmath>
#include <array>

#include "stb_truetype.h"

#include "base/file.h"
#include "base/utf8.h"
#include "base/rect.h"
#include "base/c.h"

#include "api/image.h"

#include "apiact/texture.h"
#include "apiact/log.h"

#include "Inconsolata-Medium.ttf.h"

namespace ride::apigl
{

///////////////////////////////////////////////////////////////////////////////////////////////////
// Loaded FontData

struct LoadedFontData
{
    const unsigned char* data;
    std::vector<unsigned char> data_storage;  // ttf data
    stbtt_fontinfo stbfont;
    Px size = Px{0};
    Px height = Px{0};
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// GlyphSet

float c_dip_to_stb_size(Px d)
{
    return static_cast<float>(d.value);
}

// "mirror" of stbtt_bakedchar
void Glyph::make_invisible()
{
    x1 = x0;
}

Glyph* GlyphSet::get_glyph(unsigned int c)
{
    return &glyphs[c & 0xff];
}

bool GlyphSet::load_single_glyphset_or_fail(LoadedFontData* font, int idx, int width, int height)
{
    std::vector<std::uint8_t> pixels;
    pixels.resize(Cs(width) * Cs(height));

    /* load glyphs */
    const float s = stbtt_ScaleForMappingEmToPixels(&font->stbfont, 1) /
                    stbtt_ScaleForPixelHeight(&font->stbfont, 1);

    std::array<stbtt_bakedchar, NUM_CHARS> stb_glyphs;
    const int res = stbtt_BakeFontBitmap(font->data, 0, c_dip_to_stb_size(font->size) * s, &pixels[0], width,
                                         height, idx * NUM_CHARS, NUM_CHARS, stb_glyphs.data());

    if (res < 0)
    {
        return false;
    }

    /* adjust glyph yoffsets and xadvance */
    int ascent, descent, linegap;
    stbtt_GetFontVMetrics(&font->stbfont, &ascent, &descent, &linegap);
    const auto scale = stbtt_ScaleForMappingEmToPixels(&font->stbfont, c_dip_to_stb_size(font->size));
    const auto scaled_ascent = static_cast<float>(ascent) * scale + 0.5f;
    for (int i = 0; i < NUM_CHARS; i++)
    {
        this->glyphs[Cs(i)].x0   = stb_glyphs[Cs(i)].x0;
        this->glyphs[Cs(i)].x1   = stb_glyphs[Cs(i)].x1;
        this->glyphs[Cs(i)].y0   = stb_glyphs[Cs(i)].y0;
        this->glyphs[Cs(i)].y1   = stb_glyphs[Cs(i)].y1;
        this->glyphs[Cs(i)].xoff = static_cast<int>(stb_glyphs[Cs(i)].xoff);
        this->glyphs[Cs(i)].yoff = static_cast<int>(stb_glyphs[Cs(i)].yoff + scaled_ascent);
        this->glyphs[Cs(i)].xadvance = static_cast<int>(floor(stb_glyphs[Cs(i)].xadvance));
    }

    /* convert 8bit data to 32bit */
    Image image;
    image.setup(width, height);
    for (int y = 0; y < height; y += 1)
    {
        for (int x = 0; x < width; x += 1)
        {
            uint8_t n = pixels[Cs(y * width + x)];
            image.set_color(x, y, Color::rgb(255, 255, 255, n));
        }
    }

    this->texture = std::make_shared<Texture>
    (
        image.pixels.data(), image.width, image.height,
        TextureEdge::clamp,
        TextureRenderStyle::pixel,
        Transparency::include
    );
    loaded = true;
    return true;
}

void GlyphSet::load_glyphset(LoadedFontData* font, int idx)
{
    int width = 128;
    int height = 128;

    while (load_single_glyphset_or_fail(font, idx, width, height) == false)
    {
        width *= 2;
        height *= 2;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// FontImpl
FontImpl::FontImpl() = default;
FontImpl::~FontImpl() = default;

Px FontImpl::get_data_height() const
{
    return data->height;
}

GlyphSet* FontImpl::get_glyphset(unsigned int codepoint)
{
    int idx = (codepoint >> 8) % MAX_GLYPHSET;
    if (sets[idx].loaded == false)
    {
        sets[idx].load_glyphset(data.get(), idx);
    }
    return &sets[idx];
}

Glyph* FontImpl::get_glyph(unsigned int codepoint)
{
    return get_glyphset(codepoint)->get_glyph(codepoint);
}

void FontImpl::mark_as_unloaded()
{
    for(int i=0; i<MAX_GLYPHSET; i+=1)
    {
        sets[i].loaded = false;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Font

Font::Font() = default;

Font::~Font() = default;


bool Font::load_font(const embedded_binary& data, Px size)
{
    return impl_load_font(std::make_unique<FontImpl>(), reinterpret_cast<const unsigned char*>(data.data), size);
}

bool Font::load_font(const std::string& filename, Px size)
{
    if(filename == default_font)
    {
        return load_font(INCONSOLATA_MEDIUM_TTF, size);
    }

    auto font = std::make_unique<FontImpl>();

    if (read_to_buffer(filename, &font->data->data_storage) == false)
    {
        return false;
    }

    const unsigned char* data = font->data->data_storage.data();
    return impl_load_font(std::move(font), data, size);
}

void set_size_for_font(FontImpl* font, Px size)
{
    int ascent, descent, linegap;
    stbtt_GetFontVMetrics(&font->data->stbfont, &ascent, &descent, &linegap);
    const float scale = stbtt_ScaleForMappingEmToPixels(&font->data->stbfont, c_dip_to_stb_size(size));
    const auto height = static_cast<float>(ascent - descent + linegap);
    font->data->height = Px{static_cast<int>(height * scale + 0.5f)};
}

bool Font::impl_load_font(std::unique_ptr<FontImpl> font, const unsigned char* data, Px size)
{
    /* init font */
    font->data = std::make_unique<LoadedFontData>();
    font->data->data = data;
    font->data->size = size;

    /* init stbfont */
    const auto ok = stbtt_InitFont(&font->data->stbfont, data, 0);
    if (!ok)
    {
        return false;
    }

    /* get height and scale */
    set_size_for_font(font.get(), size);

    /* make tab and newline glyphs invisible */
    font->get_glyph('\t')->make_invisible();
    font->get_glyph('\n')->make_invisible();

    m = std::move(font);
    return true;
}

void Font::set_size(Px new_size)
{
    m->data->size = new_size;
    set_size_for_font(m.get(), new_size);
    m->mark_as_unloaded();
}

void Font::set_tab_width(Px n)
{
    m->get_glyph('\t')->xadvance = n.value;
}

Px Font::get_tab_width()
{
    return Px{m->get_glyph('\t')->xadvance};
}

Px Font::get_width(const std::string& text)
{
    int x = 0;
    const auto codepoints = utf8_to_codepoints(text);
    for (const auto codepoint : codepoints)
    {
        x += m->get_glyph(codepoint)->xadvance;
    }
    return Px{x};
}

Px Font::get_height() const
{
    return m->data->height;
}

}

