#include "api/image.h"

#include <cstring> // memcpy

#include "stb_image.h"

#include "base/assert.h"

bool Image::load(const embedded_binary& data)
{
    int requested_width = 0;
    int requested_height = 0;
    int requested_channels = 0;

    unsigned char* loaded_pixels = stbi_load_from_memory
    (
        reinterpret_cast<const unsigned char*>(data.data), data.size,
        &requested_width, &requested_height, &requested_channels,
        STBI_rgb_alpha
    );
    if(loaded_pixels == nullptr)
    {
        return false;
    }

    setup(requested_width, requested_height);
    std::memcpy(pixels.data(), loaded_pixels, sizeof(std::uint8_t) * requested_width * requested_height * 4);
    stbi_image_free(loaded_pixels);
    return true;
}

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
    xassert(x >= 0 && x < width, x << " " << width);
    xassert(y >= 0 && y < height, y << " " << height);
    const auto index = (x + y * width) * 4;
    return Color::rgb(pixels[index + 0], pixels[index + 1], pixels[index + 2], pixels[index + 3]);
}

std::shared_ptr<Image> load_shared(const embedded_binary& data)
{
    auto r = std::make_shared<Image>();
    const auto loaded = r->load(data);
    if(loaded == false)
    {
        return nullptr;
    }
    else
    {
        return r;
    }
}
