#pragma once

#include <memory>

#include "core/rect.h"
#include "core/color.h"
#include "core/units.h"

#include "api/texture.h"


struct Font;

enum class Submit
{
    no, yes
};

struct Renderer
{
    Renderer() = default;
    virtual ~Renderer() = default;

    virtual void draw_rect(const Rect<Px>& rect, Color color) = 0;

    virtual void push_clip_rect(const Rect<Px>& r) = 0;
    virtual void pop_clip_rect() = 0;

    void draw_image(std::shared_ptr<Texture> texture, Px x, Px y, Color tint, std::optional<Rectf> sub = {}, Submit submit=Submit::yes);
    virtual void draw_image(std::shared_ptr<Texture> texture, const Rect<Px>& rect, Color tint, std::optional<Rectf> sub = {}, Submit submit=Submit::yes) = 0;

    virtual void submit_renderer() = 0;
};

struct ClipScope
{
    Renderer* cache;

    ClipScope(Renderer* c, const Rect<Px>& r);
    ~ClipScope();

    void clear();

    ClipScope(const ClipScope&) = delete;
    ClipScope& operator = (const ClipScope&) = delete;

    ClipScope(ClipScope&&) noexcept;
    ClipScope& operator=(ClipScope&&) noexcept;
};

