#pragma once

#include <memory>

#include "api/units.h"
#include "api/font.h"

#include "libride/document.h"

#include "ride/view.h"

struct ViewDoc : View
{
    Document doc;
    std::shared_ptr<Font> font;

    pix calculate_line_height();
    pix get_document_height();
    pix get_document_width();

    scroll_size calculate_scroll_size() override;
    void draw_body(const rect<pix>& main_view_rect, RenCache* cache) override;
};
