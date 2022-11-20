#pragma once

#include <optional>
#include <memory>

#include "embed/types.h"

#include "base/ints.h"

#include "api/texture.h"

#include "apigl/uniform.h"
// #include "apigl/dependency_opengl.h"


namespace ride::apigl
{

enum class TextureEdge
{
    clamp, repeat
};


enum class TextureRenderStyle
{
    pixel, smooth
};


enum class Transparency
{
    include, exclude
};


struct Texture : ::Texture
{
    unsigned int id;
    int width;
    int height;

    int get_width() const override;
    int get_height() const override;

    Texture(); // invalid texture

    // "internal"
    Texture
    (
        void* pixel_data, int w, int h,
        TextureEdge te,
        TextureRenderStyle trs,
        Transparency t
    );

    ~Texture();


    Texture(const Texture&) = delete;
    void operator=(const Texture&) = delete;

    Texture(Texture&&);
    void operator=(Texture&&);

    // clears the loaded texture to a invalid texture
    void unload();
};


// set the texture for the specified uniform
void
bind_texture(const Uniform& uniform, const Texture& texture);

std::optional<Texture>
load_image_from_embedded
(
    const embedded_binary& image_binary,
    TextureEdge te,
    TextureRenderStyle trs,
    Transparency t
);

Texture
load_image_from_color
(
    u32 pixel,
    TextureEdge te,
    TextureRenderStyle trs,
    Transparency t
);

std::shared_ptr<Texture>
load_shared_texture_impl(const embedded_binary& image_binary);

}


