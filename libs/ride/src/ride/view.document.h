#pragma once

#include <memory>

#include "base/minmax.h"

#include "api/units.h"
#include "api/font.h"

#include "libride/document.h"

#include "ride/view.h"


struct ViewDoc : View, VirtualView
{
    ViewDoc();
    
    std::shared_ptr<Font> font;
    rect<pix> view_rect = rect<pix>(0_px);
    rect<pix> gutter_rect = rect<pix>(0_px);
    bool dragging = false;

    pix calculate_line_height();
    pix get_full_document_height();
    pix get_full_document_width();
    pix get_gutter_width();
    minmax<int> get_line_range();
    position translate_view_position(const vec2<pix>& p);

    vec2<pix> position_to_upper_left_pix(const position& p);
    vec2<pix> position_to_lower_right_pix(const position& p);

    pix offset_to_relative_left_pix(int line_index, int offset);
    pix offset_to_relative_right_pix(int line_index, int offset);
    pix line_to_relative_upper_pix(int line_index);
    pix line_to_relative_lower_pix(int line_index);

    void draw_single_line
    (
        RenCache* cache,
        int line_index,
        const vec2<pix>& position
    );
    void drag_to(const Meta& meta, const vec2<pix>& new_mouse);

    void scroll_to_cursor(const position& p) override;

    void on_layout_body() override;
    scroll_size calculate_scroll_size() override;
    void draw_body(RenCache* cache) override;
    void on_mouse_pressed(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse, int clicks) override;
    void on_mouse_moved(const Meta& meta, const vec2<pix>& new_mouse) override;
    void on_mouse_released(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse) override;
    
};
