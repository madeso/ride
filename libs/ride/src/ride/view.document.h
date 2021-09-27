#pragma once

#include <memory>

#include "base/minmax.h"

#include "api/units.h"
#include "api/font.h"

#include "libride/document.h"

#include "ride/view.h"


struct ViewDoc : View
{
    Document doc;
    std::shared_ptr<Font> font;

    pix calculate_line_height();
    pix get_full_document_height();
    pix get_full_document_width();
    pix get_gutter_width();

    rect<pix> view_rect = rect<pix>(0_px);
    rect<pix> gutter_rect = rect<pix>(0_px);

    void on_layout_body() override;

    minmax<int> get_line_range();
    position translate_view_position(const vec2<pix>& p);

    scroll_size calculate_scroll_size() override;
    void draw_body(RenCache* cache) override;
    void on_mouse_pressed(MouseButton button, pix x, pix y, int clicks) override;
};
