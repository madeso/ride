#include "apiact/app.h"

#include <algorithm>

// #include "apiact/dependency_sdl.h"

#include "assert/assert.h"

#include "core/vec2.h"

#include "apiact/font.h"
#include "apiact/texture.h"

namespace ride::apigl
{

struct PlatformImpl : ::Platform
{
    double get_default_scale() override
    {
        return 1.0;
    }

    std::shared_ptr<::Font> make_font() override
    {
        // return std::make_shared<Font>();
        DIE("not implemented");
        return nullptr;
    }

    std::shared_ptr<::Texture> load_shared_texture(const embedded_binary&) override
    {
        // return load_shared_texture_impl(image_binary);
        DIE("not implemented");
        return nullptr;
    }
};

PlatformArg create_platform()
{
    return std::make_shared<PlatformImpl>();
}

}
