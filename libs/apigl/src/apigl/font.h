#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <array>

#include "embed/types.h"

#include "base/units.h"

#include "api/font.h"

struct Color;

namespace ride::apigl
{

struct Texture;

constexpr int MAX_GLYPHSET = 256;
constexpr int NUM_CHARS = 256;


struct LoadedFontData;

// "mirror" of stbtt_bakedchar
struct Glyph
{
    int x0; int y0;
    int x1; int y1;

    int xoff; int yoff;
    int xadvance;

    void make_invisible();
};

struct GlyphSet
{
    bool loaded = false;
    std::shared_ptr<Texture> texture;
    std::array<Glyph, NUM_CHARS> glyphs;

    Glyph* get_glyph(unsigned int c);
    bool load_single_glyphset_or_fail(LoadedFontData* font, int idx, int width, int height);
    void load_glyphset(LoadedFontData* font, int idx);
};

struct FontImpl
{
    std::unique_ptr<LoadedFontData> data;
    GlyphSet sets[MAX_GLYPHSET];

    FontImpl();
    ~FontImpl();

    GlyphSet* get_glyphset(unsigned int codepoint);
    Glyph* get_glyph(unsigned int codepoint);
    void mark_as_unloaded();

    Px get_data_height() const;
};

struct Font : ::Font
{
    Font();
    ~Font();

    std::unique_ptr<FontImpl> m;

    bool load_font(const std::string& filename, Px size) override;

    bool load_font(const embedded_binary& data, Px size);
    bool impl_load_font(std::unique_ptr<FontImpl> f, const unsigned char* data, Px size);

    void set_size(Px new_size);

    void set_tab_width(Px n);
    Px get_tab_width();

    Px get_width(const std::string& text) override;
    Px get_height() const override;
};

}

