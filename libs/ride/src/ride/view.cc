#include "ride/view.h"

#include "base/cc.h"

#include "api/app.h"
#include "api/rencache.h"


#include "ride/theme.h"

View::View()
    : app(nullptr)
    , theme(nullptr)
    , client_rect
    {
        {50_px, 50_px},
        {300_px, 300_px}
    }
    , scroll{0_px, 0_px}
    , body_rect(0_px)
{
}

std::optional<scrollbar_data> layout_scrollbar(rect<pix>* view_rect, Theme* theme,
    pix document_height, pix current_scroll,
    bool is_vertical, Side through_side, Side top_side, Side bottom_side)
{
    const auto view_rect_width = view_rect->width;
    const auto view_rect_height = view_rect->height;

    const auto view_prop = is_vertical ? view_rect_height : view_rect_width;

    if(view_prop >= document_height)
    {
        return {};
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
    
    return scrollbar_data
    {
        through_rect,
        thumb_rect,
        top_button_rect,
        bottom_button_rect
    };
}

void draw_scrollbar(const scrollbar_data& data, App* app, Theme* theme, Renderer* cache)
{
    const auto clip_scope = ClipScope{cache, app->to_dip(data.through_rect)};
    draw_rect(cache, app->to_dip(data.through_rect), theme->scroll_through_color);
    draw_rect(cache, app->to_dip(data.thumb_rect), theme->scroll_thumb_color);
    draw_rect(cache, app->to_dip(data.top_button_rect), theme->scroll_button_color);
    draw_rect(cache, app->to_dip(data.bottom_button_rect), theme->scroll_button_color);
}

void View::on_layout(const rect<pix>& new_client_rect)
{
    client_rect = new_client_rect;
    body_rect = new_client_rect;

    const auto scroll_size = get_scroll_size();
    
    if(scroll_size.height)
    {
        vertical_scrollbar_data = layout_scrollbar
        (
            &body_rect, theme, *scroll_size.height, scroll.y,
            true, get_vertical_scroll_side(*theme), Side::top, Side::bottom
        );
    }

    if(scroll_size.width)
    {
        horizontal_scrollbar_data = layout_scrollbar
        (
            &body_rect, theme,
            *scroll_size.width, scroll.x,
            false, get_horizontal_scroll_side(*theme), Side::left, Side::right
        );
    }

    on_layout_body();
}


void View::on_layout_body()
{
}


std::optional<pix> keep_above_zero(std::optional<pix> p)
{
    if(p.has_value() == false) { return p; }

    if(p->value > 0) { return p; }
    else return 0_px;
}

scroll_size View::get_scroll_size()
{
    const auto r = calculate_scroll_size();
    return
    {
        keep_above_zero(r.width),
        keep_above_zero(r.height)
    };
}

void View::draw(Renderer* cache)
{
    if(vertical_scrollbar_data)
    {
        draw_scrollbar(*vertical_scrollbar_data, app, theme, cache);
    }

    if(horizontal_scrollbar_data)
    {
        draw_scrollbar(*horizontal_scrollbar_data, app, theme, cache);
    }

    const auto clip_scope = ClipScope{cache, app->to_dip(body_rect)};
    draw_body(cache);
}

void do_scroll(pix* scroll, const std::optional<pix>& size, int d, pix change)
{
    if(size)
    {
        if(d != 0)
        {
            *scroll = keep_within
            (
                0_px,
                *scroll - static_cast<double>(d) * change,
                *size
            );
        }
        else
        {
            *scroll = keep_within
            (
                0_px,
                *scroll,
                *size
            );
        }
    }
    else
    {
        // if not scroll is requested, make sure the scroll is zero
        *scroll = 0_px;
    }
    
}

void View::on_mouse_wheel(int dx, int dy)
{
    const auto scroll_size = get_scroll_size();

    do_scroll(&scroll.x, scroll_size.width, dx, theme->horizontal_scroll);
    do_scroll(&scroll.y, scroll_size.height, dy, theme->vertical_scroll);
}

void View::keep_scroll_within()
{
    on_mouse_wheel(0, 0);
}

void View::on_mouse_pressed(MouseButton, const Meta&, const vec2<pix>&, int)
{
}

void View::on_mouse_moved(const Meta&, const vec2<pix>&)
{
}

void View::on_mouse_released(MouseButton, const Meta&, const vec2<pix>&)
{
}

void View::on_text(const std::string&)
{
}
