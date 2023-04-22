#pragma once

#include <memory>

#include "core/minmax.h"
#include "core/units.h"

#include "api/font.h"

#include "ride/document.h"
#include "ride/view.h"


namespace ride::libride
{



struct ViewDoc : LineView, VirtualView
{
    ViewDoc();
    
    Rect<Dp> gutter_rect = Rect<Dp>(0_dp);
    bool dragging = false;
    std::size_t gutter_char_length = 1;

    Vec2<Dp> last_mouse = {0_dp, 0_dp};

    void draw_line(api::Renderer* cache, std::size_t index, const Dp& x, const Dp& y) override;
    Dp get_document_width() const override;
    std::size_t get_number_of_lines() const override;

    Dp calculate_line_height();
    Dp get_full_document_height();
    Dp get_full_document_width() const;
    MinMax<int> get_line_range();
    Position translate_view_position(const Vec2<Dp>& p);

    Vec2<Dp> position_to_upper_left_pix(const Position& p);
    Vec2<Dp> position_to_lower_right_pix(const Position& p);

    Dp offset_to_relative_left_pix(int line_index, int offset);
    Dp offset_to_relative_right_pix(int line_index, int offset);
    Dp line_to_relative_upper_pix(int line_index);
    Dp line_to_relative_lower_pix(int line_index);

    int absolute_pix_x_to_offset(int line, Dp x);

    void draw_single_line
    (
        api::Renderer* cache,
        int line_index,
        const Vec2<Dp>& position
    );
    void drag_to(const api::Meta& meta, const Vec2<Dp>& new_mouse);

    void scroll_to_cursor(const Position& p) override;
    Dp get_relative_pixel_offset(const Position& p) override;
    int get_offset_from_relative_pixel_offset(int line, Dp offset) override;

    void on_layout_body() override;
    // ScrollSize calculate_scroll_size() override;
    void draw_body(api::Renderer* cache) override;
    void on_mouse_pressed(api::MouseButton button, const api::Meta& meta, const Vec2<Dp>& new_mouse, int clicks) override;
    void on_mouse_moved(const api::Meta& meta, const Vec2<Dp>& new_mouse) override;
    void on_mouse_released(api::MouseButton button, const api::Meta& meta, const Vec2<Dp>& new_mouse) override;
    void on_text(const std::string& t) override;
};


}

