#pragma once

#include <memory>

#include "base/minmax.h"

#include "base/units.h"
#include "api/font.h"

#include "libride/document.h"

#include "ride/view.h"


struct ViewDoc : LineView, VirtualView
{
    ViewDoc();
    
    rect<pix> gutter_rect = rect<pix>(0_px);
    bool dragging = false;
    std::size_t gutter_char_length = 1;

    vec2<pix> last_mouse = {0_px, 0_px};

    void draw_line(Renderer* cache, std::size_t index, const pix& x, const pix& y) override;
    pix get_document_width() const override;
    std::size_t get_number_of_lines() const override;

    pix calculate_line_height();
    pix get_full_document_height();
    pix get_full_document_width() const;
    minmax<int> get_line_range();
    position translate_view_position(const vec2<pix>& p);

    vec2<pix> position_to_upper_left_pix(const position& p);
    vec2<pix> position_to_lower_right_pix(const position& p);

    pix offset_to_relative_left_pix(int line_index, int offset);
    pix offset_to_relative_right_pix(int line_index, int offset);
    pix line_to_relative_upper_pix(int line_index);
    pix line_to_relative_lower_pix(int line_index);

    int absolute_pix_x_to_offset(int line, pix x);

    void draw_single_line
    (
        Renderer* cache,
        int line_index,
        const vec2<pix>& position
    );
    void drag_to(const Meta& meta, const vec2<pix>& new_mouse);

    void scroll_to_cursor(const position& p) override;
    pix get_relative_pixel_offset(const position& p) override;
    int get_offset_from_relative_pixel_offset(int line, pix offset) override;

    void on_layout_body() override;
    // ScrollSize calculate_scroll_size() override;
    void draw_body(Renderer* cache) override;
    void on_mouse_pressed(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse, int clicks) override;
    void on_mouse_moved(const Meta& meta, const vec2<pix>& new_mouse) override;
    void on_mouse_released(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse) override;
    void on_text(const std::string& t) override;
};
