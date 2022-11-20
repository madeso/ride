#include "api/renderer.h"

void Renderer::draw_image(std::shared_ptr<Texture> texture, Px x, Px y, Color c, std::optional<Rectf> sub, Submit submit)
{
    draw_image
    (
        texture,
        {
            x,
            y,
            Px{texture->get_width()},
            Px{texture->get_height()}
        },
        c,
        sub,
        submit
    );
}

ClipScope::ClipScope(Renderer* c, const Rect<Px>& r)
    : cache(c)
{
    cache->push_clip_rect(r);
}

ClipScope::~ClipScope()
{
    clear();
}

void ClipScope::clear()
{
    if (cache == nullptr)
    {
        return;
    }
    cache->pop_clip_rect();
}

ClipScope::ClipScope(ClipScope&& rhs) noexcept
    : cache(rhs.cache)
{
    rhs.cache = nullptr;
}

ClipScope& ClipScope::operator = (ClipScope&& rhs) noexcept
{
    clear();
    cache = rhs.cache;
    rhs.cache = nullptr;
    return *this;
}

