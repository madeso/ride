#pragma once

#include <memory>
#include <array>
#include <functional>

#include "embed/types.h"

#include "core/units.h"

#include "api/font.h"

struct Color;
struct Image;

namespace ride::backends::sdl
{

constexpr int MAX_GLYPHSET = 256;
constexpr int NUM_CHARS = 256;

struct LoadedTexture
{
    LoadedTexture() = default;
    virtual ~LoadedTexture() = default;

    LoadedTexture(const LoadedTexture&) = delete;
    LoadedTexture(LoadedTexture&&) = delete;
    void operator=(const LoadedTexture&) = delete;
    void operator=(LoadedTexture&&) = delete;
};

struct FontBase;

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
    std::shared_ptr<LoadedTexture> texture;
    std::array<Glyph, NUM_CHARS> glyphs;

    Glyph* get_glyph(unsigned int c);
    bool load_single_glyphset_or_fail(LoadedFontData* font, int idx, int width, int height, const FontBase& loader);
    void load_glyphset(LoadedFontData* font, int idx, const FontBase& loader);
};

struct FontImpl
{
    std::unique_ptr<LoadedFontData> data;
    GlyphSet sets[MAX_GLYPHSET];

    FontImpl();
    ~FontImpl();

    GlyphSet* get_glyphset(unsigned int codepoint, const FontBase& loader);
    Glyph* get_glyph(unsigned int codepoint, const FontBase& loader);
    void mark_as_unloaded();

    Px get_data_height() const;
};

struct FontBase : Font
{
    FontBase();
    virtual ~FontBase();

    std::unique_ptr<FontImpl> m;

    bool load_font(const std::string& filename, Px size) override;

    bool load_font(const embedded_binary& data, Px size);
    bool impl_load_font(std::unique_ptr<FontImpl> f, const unsigned char* data, Px size);

    void set_size(Px new_size);

    void set_tab_width(Px n);
    Px get_tab_width();

    Px get_width(const std::string& text) override;
    Px get_height() const override;

    virtual std::shared_ptr<LoadedTexture> load_texture(const Image& img) const = 0;
};

}

