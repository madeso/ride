#pragma once

#include <optional>
#include <memory>

#include "base/vec2.h"
#include "base/size.h"
#include "base/rect.h"
#include "base/cursor.h"
#include "base/units.h"

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
    std::optional<pix> width;
    std::optional<pix> height;
};

struct ScrollbarData
{
    Rect<pix> through_rect;
    Rect<pix> thumb_rect;
    Rect<pix> top_button_rect;
    Rect<pix> bottom_button_rect;
};

struct View
{
    App* app;
    Theme* theme;

    CursorType cursor = CursorType::arrow;

    // includes scrollbar
    Rect<pix> client_rect;

    Vec2<pix> scroll;

    // excluding scrollbar
    Rect<pix> body_rect;
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

    void on_layout(const Rect<pix>& new_client_rect);

    virtual void on_mouse_pressed(MouseButton button, const Meta& meta, const Vec2<pix>& new_mouse, int clicks);
    virtual void on_mouse_moved(const Meta& meta, const Vec2<pix>& new_mouse);
    virtual void on_mouse_released(MouseButton button, const Meta& meta, const Vec2<pix>& new_mouse);
    virtual void on_text(const std::string& t);
};


struct LineView : View
{
    std::shared_ptr<Font> font;
    Rect<pix> view_rect = Rect<pix>(0_px);

    void on_layout_body() override;

    Rect<pix> hit_rect_for_line(const pix& ypos) const;

    pix calculate_line_height() const;
    pix line_number_to_y(std::size_t line) const;
    ScrollSize calculate_scroll_size() override;
    std::optional<std::size_t> get_index_under_view_position(const Vec2<pix> relative_mouse);

    void draw_lines(Renderer* cache);
    int absolute_pix_y_to_line(pix y);

    virtual void draw_line(Renderer* cache, std::size_t index, const pix& x, const pix& y) = 0;
    virtual pix get_document_width() const = 0;
    virtual std::size_t get_number_of_lines() const = 0;
};
