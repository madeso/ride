#pragma once

#include <memory>
#include <optional>

#include "base/rect.h"
#include "base/color.h"
#include "base/units.h"

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

    virtual void draw_rect(const Rect<dip>& rect, Color color) = 0;
    virtual dip draw_text(std::shared_ptr<Font> font, const std::string& text, dip x, dip y, Color color) = 0;

    virtual void push_clip_rect(const Rect<dip>& r) = 0;
    virtual void pop_clip_rect() = 0;

    void draw_image(std::shared_ptr<Texture> texture, dip x, dip y, Color tint, std::optional<Rectf> sub = {}, Submit submit=Submit::yes);
    virtual void draw_image(std::shared_ptr<Texture> texture, const Rect<dip>& rect, Color tint, std::optional<Rectf> sub = {}, Submit submit=Submit::yes) = 0;

    virtual void submit_renderer() = 0;
};

struct ClipScope
{
    Renderer* cache;

    ClipScope(Renderer* c, const Rect<dip>& r);
    ~ClipScope();

    void clear();

    ClipScope(const ClipScope&) = delete;
    ClipScope& operator = (const ClipScope&) = delete;

    ClipScope(ClipScope&&) noexcept;
    ClipScope& operator=(ClipScope&&) noexcept;
};

