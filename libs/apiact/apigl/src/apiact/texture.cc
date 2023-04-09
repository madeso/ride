#include "apiact/texture.h"

#include "stb_image.h"

#include "assert/assert.h"

#include "core/c.h"

#include "apiact/dependency_opengl.h"
#include "apiact/log.h"


namespace ride::apigl
{

namespace
{
    constexpr unsigned int invalid_id = 0;
    constexpr int invalid_size = -1;

    unsigned int create_texture()
    {
        unsigned int texture;
        glGenTextures(1, &texture);
        return texture;
    }
}


Texture::Texture()
    : id(invalid_id)
    , width(invalid_size)
    , height(invalid_size)
{
}

int Texture::get_width() const
{
    return width;
}

int Texture::get_height() const
{
    return height;
}

Texture::Texture
(
    void* pixel_data, int w, int h,
    TextureEdge te,
    TextureRenderStyle trs,
    Transparency t
)
    : id(create_texture())
    , width(w)
    , height(h)
{
    glBindTexture(GL_TEXTURE_2D, id);

    const auto wrap = te == TextureEdge::clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

    const auto render_pixels = trs == TextureRenderStyle::pixel;

    const auto min_filter = render_pixels ? GL_NEAREST : GL_LINEAR_MIPMAP_LINEAR;
    const auto mag_filter = render_pixels ? GL_NEAREST : GL_LINEAR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

    const auto include_transparency = t == Transparency::include;

    if(pixel_data == nullptr)
    {
        LOG_ERROR("ERROR: Failed to load image.");
        unload();
    }
    else
    {
        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            include_transparency ? GL_RGBA : GL_RGB,
            width, height,
            0,
            include_transparency ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
            pixel_data
        );
        if(render_pixels == false)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
}


Texture::~Texture()
{
    unload();
}


Texture::Texture(Texture&& rhs)
    : id(rhs.id)
    , width(rhs.width)
    , height(rhs.height)
{
    rhs.id = invalid_id;
    rhs.width = invalid_size;
    rhs.height = invalid_size;
}


void
Texture::operator=(Texture&& rhs)
{
    unload();

    id = rhs.id;
    width = rhs.width;
    height = rhs.height;

    rhs.id = invalid_id;
    rhs.width = invalid_size;
    rhs.height = invalid_size;
}


void
Texture::unload()
{
    if(id != invalid_id)
    {
        glDeleteTextures(1, &id);
        id = invalid_id;
    }

    width = invalid_size;
    height = invalid_size;
}


void
bind_texture(const Uniform& uniform, const Texture& texture)
{
    if(uniform.is_valid() == false) { return; }
    ASSERT(uniform.texture >= 0);

    glActiveTexture(Cint_to_glenum(GL_TEXTURE0 + uniform.texture));
    glBindTexture(GL_TEXTURE_2D, texture.id);
}


std::optional<Texture>
LoadImage
(
    const unsigned char* image_source,
    int size,
    TextureEdge te,
    TextureRenderStyle trs,
    Transparency t
)
{
    const auto include_transparency = t == Transparency::include;

    int width = 0;
    int height = 0;
    int junk_channels = 0;

    stbi_set_flip_vertically_on_load(true);
    auto* pixel_data = stbi_load_from_memory
    (
        image_source, size,
        &width, &height,
        &junk_channels, include_transparency ? 4 : 3
    );

    if(pixel_data == nullptr)
    {
        LOG_ERROR("ERROR: Failed to load image from image source");
        return {};
    }

    auto loaded = Texture{pixel_data, width, height, te, trs, t};

    if(pixel_data != nullptr)
    {
        stbi_image_free(pixel_data);
    }

    return loaded;
}


std::optional<Texture>
load_image_from_embedded
(
    const embedded_binary& image_binary,
    TextureEdge te,
    TextureRenderStyle trs,
    Transparency t
)
{
    return LoadImage
    (
        reinterpret_cast<const unsigned char*>(image_binary.data),
        c_uint_to_int(image_binary.size),
        te,
        trs,
        t
    );
}


Texture
load_image_from_color
(
    u32 pixel,
    TextureEdge te,
    TextureRenderStyle trs,
    Transparency t
)
{
    return
    {
        &pixel, 1, 1, te, trs, t
    };
}


std::shared_ptr<Texture>
load_shared_texture_impl(const embedded_binary& image_binary)
{
    auto texture = load_image_from_embedded(image_binary, TextureEdge::clamp, TextureRenderStyle::smooth, Transparency::include);
    if(texture)
    {
        return std::make_shared<Texture>(std::move(*texture));
    }
    else
    {
        return nullptr;
    }
}

}


