#pragma once

#include <optional>

#include "base/vec2.h"
#include "base/size.h"
#include "base/rect.h"

#include "api/units.h"


struct App;
struct Theme;
struct RenCache;


struct scroll_size
{
    std::optional<pix> width;
    std::optional<pix> height;
};

struct View
{
    App* app;
    Theme* theme;

    vec2<pix> position;
    size<pix> size;
    vec2<pix> scroll;

    View();
    virtual ~View() = default;

    void set_rect(const rect<pix>& r);

    void draw_scrollbar
    (
        rect<pix>* view_rect, RenCache* cache,
        pix document_height, pix scroll,
        bool is_vertical, Side through_side, Side top_side, Side bottom_side
    );

    void draw(RenCache* cache);
    void on_mouse_wheel(int dx, int dy);

    virtual void draw_body(const rect<pix>& main_view_rect, RenCache* cache) = 0;
    virtual scroll_size calculate_scroll_size() = 0;
};
