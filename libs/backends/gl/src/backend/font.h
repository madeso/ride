#pragma once

#include "backend/fontbase.h"

namespace ride::backends::gl
{
struct Texture;

struct LoadedGlTexture : sdl::LoadedTexture
{
    LoadedGlTexture() = default; 
    std::shared_ptr<Texture> texture;

    LoadedGlTexture(const LoadedGlTexture&) = delete;
    LoadedGlTexture(LoadedGlTexture&&) = delete;
    void operator=(const LoadedGlTexture&) = delete;
    void operator=(LoadedGlTexture&&) = delete;
};

struct Font : sdl::FontBase
{
    std::shared_ptr<sdl::LoadedTexture> load_texture(const Image& img) const override;
    std::shared_ptr<Texture> get_texture(sdl::GlyphSet* set);
};

}
