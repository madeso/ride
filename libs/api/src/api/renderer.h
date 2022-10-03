#pragma once

#include <memory>
#include <optional>

#include "base/rect.h"
#include "base/color.h"



struct Renderer;
struct Texture;

enum class Submit
{
    no, yes
};

void draw_rect(Renderer* ren, const rect<dip>& rect, Color color);
void draw_image(Renderer* ren, std::shared_ptr<Texture> texture, const rect<dip>& rect, Color tint, std::optional<Rectf> sub = {}, Submit submit=Submit::yes);
void draw_image(Renderer* ren, std::shared_ptr<Texture> texture, dip x, dip y, Color tint, std::optional<Rectf> sub = {}, Submit submit=Submit::yes);
void submit_renderer(Renderer* ren);

struct ClipScope
{
    Renderer* cache;

    ClipScope(Renderer* c, const rect<dip>& r);
    ~ClipScope();

    void clear();

    ClipScope(const ClipScope&) = delete;
    ClipScope& operator = (const ClipScope&) = delete;

    ClipScope(ClipScope&&) noexcept;
    ClipScope& operator=(ClipScope&&) noexcept;
};
