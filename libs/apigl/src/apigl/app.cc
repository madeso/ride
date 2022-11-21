#include "apigl/app.h"

#include <algorithm>

#include "apigl/dependency_sdl.h"

#include "base/vec2.h"

#include "apigl/font.h"
#include "apigl/texture.h"


#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#elif __linux__
#include <unistd.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#endif

namespace ride::apigl
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

struct PlatformImpl : ::Platform
{
    double get_default_scale() override
    {
        return calculate_scale();
    }

    std::shared_ptr<::Font> make_font() override
    {
        return std::make_shared<Font>();
    }

    std::shared_ptr<::Texture> load_shared_texture(const embedded_binary& image_binary) override
    {
        return load_shared_texture_impl(image_binary);
    }
};

PlatformArg create_platform()
{
    return std::make_shared<PlatformImpl>();
}

}

