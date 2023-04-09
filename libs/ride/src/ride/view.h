#pragma once

#include <memory>

#include "core/vec2.h"
#include "core/size.h"
#include "core/rect.h"
#include "core/cursor.h"
#include "core/units.h"

#include "api/key.h"
#include "api/font.h"
#include "api/mouse_button.h"


struct App;
struct Theme;
struct Renderer;

/*

         scroll_size
       (0,0)
 +       +---------+       |
 |       |         |       |
 |       |         |       | scroll (positive)
 |       |         |       |
 |       |         |       |
 |       |         |       V
 |    +---------------+    +
 |    |  |         |  |    |
 |    |  |         |  |    | client/body
 |    |  |         |  |    |
 |    +---------------+    +
 |       |         |
 |       |         |
 |       |         |
 |       |         |
 |       |         |
 +       +---------+
      (0,-height)

*/


struct ScrollSize
{
    std::optional<Dp> width;
    std::optional<Dp> height;
};

struct ScrollbarData
{
    Rect<Dp> through_rect;
    Rect<Dp> thumb_rect;
    Rect<Dp> top_button_rect;
    Rect<Dp> bottom_button_rect;
};

struct View
{
    App* app;
    Theme* theme;

    CursorType cursor = CursorType::arrow;

    // includes scrollbar
    Rect<Dp> client_rect;

    Vec2<Dp> scroll;

    // excluding scrollbar
    Rect<Dp> body_rect;
    std::optional<ScrollbarData> vertical_scrollbar_data;
    std::optional<ScrollbarData> horizontal_scrollbar_data;

    // get body rect in view space, (0,0) is top of unscrolled document

    void keep_scroll_within();

    View();
    virtual ~View() = default;

    ScrollSize get_scroll_size();

    virtual void draw(Renderer* cache);
    void on_mouse_wheel(int dx, int dy);

    virtual void draw_body(Renderer* cache) = 0;
    virtual ScrollSize calculate_scroll_size() = 0;

    // called after client_rect and body_rect is set
    virtual void on_layout_body();

    void on_layout(const Rect<Dp>& new_client_rect);

    virtual void on_mouse_pressed(MouseButton button, const Meta& meta, const Vec2<Dp>& new_mouse, int clicks);
    virtual void on_mouse_moved(const Meta& meta, const Vec2<Dp>& new_mouse);
    virtual void on_mouse_released(MouseButton button, const Meta& meta, const Vec2<Dp>& new_mouse);
    virtual void on_text(const std::string& t);
};


struct LineView : View
{
    std::shared_ptr<Font> font;
    Rect<Dp> view_rect = Rect<Dp>(0_dp);

    void on_layout_body() override;

    Rect<Dp> hit_rect_for_line(const Dp& ypos) const;

    Dp calculate_line_height() const;
    Dp line_number_to_y(std::size_t line) const;
    ScrollSize calculate_scroll_size() override;
    std::optional<std::size_t> get_index_under_view_position(const Vec2<Dp> relative_mouse);

    void draw_lines(Renderer* cache);
    int absolute_pix_y_to_line(Dp y);

    virtual void draw_line(Renderer* cache, std::size_t index, const Dp& x, const Dp& y) = 0;
    virtual Dp get_document_width() const = 0;
    virtual std::size_t get_number_of_lines() const = 0;
};
