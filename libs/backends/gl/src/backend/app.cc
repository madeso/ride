#include "backend/app.h"

#include <algorithm>

#include "backend/dependency_sdl.h"

#include "core/vec2.h"

#include "api/image.h"

#include "backend/font.h"
#include "backend/texture.h"


#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#elif __linux__
#include <unistd.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#endif

namespace ride::backends::gl
{

double calculate_scale()
{
    float dpi;
    SDL_GetDisplayDPI(0, NULL, &dpi, NULL);
#if _WIN32
    return dpi / 96.0;
#else
    return 1.0;
#endif
}

struct PlatformImpl : api::Platform
{
    double get_default_scale() override
    {
        return calculate_scale();
    }

    std::shared_ptr<api::Font> make_font() override
    {
        return std::make_shared<sdl::Font>();
    }

    std::shared_ptr<api::Texture> load_shared_texture(const embedded_binary& image_binary) override
    {
        return load_shared_texture_impl(image_binary);
    }

    std::shared_ptr<api::Texture> load_texture(const api::Image& image) const override
    {
        return std::make_shared<Texture>(image.pixels.data(), image.width, image.height,
            TextureEdge::clamp, TextureRenderStyle::pixel, Transparency::include);
    }
};


api::PlatformArg create_platform()
{
    return std::make_shared<PlatformImpl>();
}

}

