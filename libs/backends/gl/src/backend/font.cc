#include "backend/font.h"

#include "api/image.h"
#include "backend/texture.h"

namespace ride::backends::gl
{
    std::shared_ptr<sdl::LoadedTexture> Font::load_texture(const Image& image) const
    {
        auto ret = std::make_shared<LoadedGlTexture>();

        ret->texture = std::make_shared<Texture>
        (
            image.pixels.data(), image.width, image.height,
            TextureEdge::clamp,
            TextureRenderStyle::pixel,
            Transparency::include
        );

        return ret;
    }

    std::shared_ptr<Texture> Font::get_texture(sdl::GlyphSet* set)
    {
        auto te = set->texture;
        if (te == nullptr) { return nullptr; }

        auto loaded = static_cast<LoadedGlTexture*>(te.get());
        return loaded->texture;
    }
}
