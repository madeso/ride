#pragma once

#include <optional>

#include "base/vec2.h"
#include "base/size.h"
#include "base/rect.h"

#include "api/key.h"

#include "api/units.h"
#include "api/mouse_button.h"


struct App;
struct Theme;
struct RenCache;


struct scroll_size
{
    std::optional<pix> width;
    std::optional<pix> height;
};

struct scrollbar_data
{
    rect<pix> through_rect;
    rect<pix> thumb_rect;
    rect<pix> top_button_rect;
    rect<pix> bottom_button_rect;
};

struct View
{
    App* app;
    Theme* theme;

    // includes scrollbar
    rect<pix> client_rect;

    vec2<pix> scroll;

    // excluding scrollbar
    rect<pix> body_rect;
    std::optional<scrollbar_data> vertical_scrollbar_data;
    std::optional<scrollbar_data> horizontal_scrollbar_data;

    // get body rect in view space, (0,0) is top of unscrolled document

    View();
    virtual ~View() = default;

    scroll_size get_scroll_size();

    void draw(RenCache* cache);
    void on_mouse_wheel(int dx, int dy);

    virtual void draw_body(RenCache* cache) = 0;
    virtual scroll_size calculate_scroll_size() = 0;
    
    // called after client_rect and body_rect is set
    virtual void on_layout_body();

    void on_layout(const rect<pix>& new_client_rect);

    virtual void on_mouse_pressed(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse, int clicks);
    virtual void on_mouse_moved(const Meta& meta, const vec2<pix>& new_mouse);
    virtual void on_mouse_released(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse);
};
