#include "ride/view.h"

#include "base/cc.h"

#include "api/app.h"
#include "api/rencache.h"


#include "ride/theme.h"

View::View()
    : position
        ({
            50_px,
            50_px
        })
    , client_size
        ({
            300_px,
            300_px
        })
    , scroll
        ({
            pix{0},
            pix{0}
        })
{
}

void View::set_rect(const rect<pix>& r)
{
    position = {r.x, r.y};
    client_size = {r.width, r.height};
}

rect<pix> View::get_rect() const
{
    return {position, client_size};
}

void View::draw_scrollbar
(
    rect<pix>* view_rect, RenCache* cache,
    pix document_height, pix current_scroll,
    bool is_vertical, Side through_side, Side top_side, Side bottom_side
)
{
    const auto view_rect_width = view_rect->width;
    const auto view_rect_height = view_rect->height;

    const auto view_prop = is_vertical ? view_rect_height : view_rect_width;

    if(view_prop >= document_height)
    {
        return;
    }

    const auto fraction = view_prop / document_height;

    const auto through_rect = view_rect->cut(through_side, theme->scrollbar_width);

    auto track_rect = through_rect;
    const auto top_button_rect = track_rect.cut(top_side, theme->scrollbar_width);
    const auto bottom_button_rect = track_rect.cut(bottom_side, theme->scrollbar_width);

    const auto scroll_fraction = current_scroll / document_height;

    const auto size_prop = is_vertical ? track_rect.height : track_rect.width;
    const auto thumb_size = size_prop * fraction;
    const auto available_size = size_prop - thumb_size;
    const auto thumb_offset = available_size * scroll_fraction;
    const auto thumb_rect = is_vertical
        ? rect<pix>{track_rect.x               , track_rect.y + thumb_offset, track_rect.width, thumb_size       }
        : rect<pix>{track_rect.x + thumb_offset, track_rect.y               , thumb_size      , track_rect.height}
        ;

    const auto clip_scope = ClipScope{cache, app->to_dip(through_rect)};
    cache->draw_rect(app->to_dip(through_rect), theme->scroll_through_color);
    cache->draw_rect(app->to_dip(thumb_rect), theme->scroll_thumb_color);
    cache->draw_rect(app->to_dip(top_button_rect), theme->scroll_button_color);
    cache->draw_rect(app->to_dip(bottom_button_rect), theme->scroll_button_color);
}

void View::draw(RenCache* cache)
{
    auto main_view_rect = rect<pix>{position, client_size};

    const auto scroll_size = calculate_scroll_size();
    
    if(scroll_size.height)
    {
        draw_scrollbar
        (
            &main_view_rect, cache,
            *scroll_size.height, scroll.y,
            true, Side::right, Side::top, Side::bottom
        );
    }

    if(scroll_size.width)
    {
        draw_scrollbar
        (
            &main_view_rect, cache,
            *scroll_size.width, scroll.x,
            false, Side::bottom, Side::left, Side::right
        );
    }

    const auto clip_scope = ClipScope{cache, app->to_dip(main_view_rect)};
    draw_body(main_view_rect, cache);
}

void do_scroll(pix* scroll, const std::optional<pix>& size, int d, pix change)
{
    if(size && d != 0)
    {
        *scroll = keep_within
        (
            pix{0},
            *scroll - static_cast<double>(d) * change,
            *size
        );
    }
}

void View::on_mouse_wheel(int dx, int dy)
{
    const auto scroll_size = calculate_scroll_size();

    do_scroll(&scroll.x, scroll_size.width, dx, theme->horizontal_scroll);
    do_scroll(&scroll.y, scroll_size.height, dy, theme->vertical_scroll);
}

void View::on_mouse_pressed(MouseButton, pix, pix, int)
{
}

void View::on_mouse_moved(const vec2<pix>&)
{
}
