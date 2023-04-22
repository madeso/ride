#include "ride/view.h"

#include "core/cc.h"

#include "api/app.h"
#include "api/renderer.h"


#include "ride/theme.h"



namespace ride::libride
{


View::View()
    : app(nullptr)
    , theme(nullptr)
    , client_rect
    {
        {50_dp, 50_dp},
        {300_dp, 300_dp}
    }
    , scroll{0_dp, 0_dp}
    , body_rect(0_dp)
{
}

std::optional<ScrollbarData> layout_scrollbar(Rect<Dp>* view_rect, Theme* theme,
    Dp document_height, Dp current_scroll,
    bool is_vertical, Side scrollbar_on_side, Side top_side, Side bottom_side)
{
    const auto view_rect_width = view_rect->width;
    const auto view_rect_height = view_rect->height;

    const auto view_prop = is_vertical ? view_rect_height : view_rect_width;

    if(view_prop >= document_height)
    {
        return {};
    }

    const auto fraction = view_prop / document_height;

    const auto through_rect = view_rect->cut(scrollbar_on_side, theme->scrollbar_width);

    auto track_rect = through_rect;
    const auto top_button_rect = track_rect.cut(top_side, theme->scrollbar_width);
    const auto bottom_button_rect = track_rect.cut(bottom_side, theme->scrollbar_width);

    /*

    +---+ ------------------------------+
    | A | top_button_rect               |
    +---+ --------------+               |
    |///|               |               |
    |///| offset        |               |
    |///|               |               |
    +---+               |               |
    |   |               |               |
    |   |               |               |
    |   | thumb_rect    | track_rect    | through_rect
    |   |               |               |
    |   |               |               |
    +---+               |               |
    |///|               |               |
    |///|               |               |
    +---+  -------------+               |
    | V | bottom_button_rect            |
    +---+ ------------------------------+

    */

    const auto scroll_fraction = current_scroll / document_height;

    const auto size_prop = is_vertical ? track_rect.height : track_rect.width;
    const auto thumb_size = size_prop * fraction;
    const auto available_size = size_prop - thumb_size;
    const auto thumb_offset = available_size * scroll_fraction;
    XASSERT(thumb_offset >= 0_dp, thumb_offset.value);

    const auto vtop = track_rect.get_top() - thumb_offset;

    const auto thumb_rect = is_vertical
        ? Rect<Dp>::from_lrtb(track_rect.get_left(), track_rect.get_right(), vtop, vtop - thumb_size)
        : Rect<Dp>::from_lrtb(track_rect.get_left() + thumb_offset, track_rect.get_left() + thumb_offset + thumb_size, track_rect.get_top(), track_rect.get_bottom())
        ;

    return ScrollbarData
    {
        through_rect,
        thumb_rect,
        top_button_rect,
        bottom_button_rect
    };
}

void draw_scrollbar(const ScrollbarData& data, api::App* app, Theme* theme, api::Renderer* cache)
{
    // const auto clip_scope = api::ClipScope{cache, app->Cpx(data.through_rect)};
    cache->draw_rect(app->Cpx(data.through_rect), theme->scroll_through_color);
    cache->draw_rect(app->Cpx(data.thumb_rect), theme->scroll_thumb_color);
    cache->draw_rect(app->Cpx(data.top_button_rect), theme->scroll_button_color);
    cache->draw_rect(app->Cpx(data.bottom_button_rect), theme->scroll_button_color);
}

void View::on_layout(const Rect<Dp>& new_client_rect)
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


std::optional<Dp> keep_above_zero(std::optional<Dp> p)
{
    if(p.has_value() == false) { return p; }

    if(p->value > 0) { return p; }
    else return 0_dp;
}

ScrollSize View::get_scroll_size()
{
    const auto r = calculate_scroll_size();
    return
    {
        keep_above_zero(r.width),
        keep_above_zero(r.height)
    };
}

void View::draw(api::Renderer* cache)
{
    if(vertical_scrollbar_data)
    {
        draw_scrollbar(*vertical_scrollbar_data, app, theme, cache);
    }

    if(horizontal_scrollbar_data)
    {
        draw_scrollbar(*horizontal_scrollbar_data, app, theme, cache);
    }

    const auto clip_scope = api::ClipScope{cache, app->Cpx(body_rect)};
    draw_body(cache);
}

void do_scroll(Dp* scroll, const std::optional<Dp>& size, int d, Dp change)
{
    if(size)
    {
        if(d != 0)
        {
            *scroll = keep_within
            (
                0_dp,
                *scroll + static_cast<double>(d) * change,
                *size
            );
        }
        else
        {
            *scroll = keep_within
            (
                0_dp,
                *scroll,
                *size
            );
        }
    }
    else
    {
        // if not scroll is requested, make sure the scroll is zero
        *scroll = 0_dp;
    }

}

void View::on_mouse_wheel(int dx, int dy)
{
    const auto scroll_size = get_scroll_size();

    // y: scroll+ up => y - scroll
    // x: scroll+ right =>x + scroll

    do_scroll(&scroll.x, scroll_size.width, dx, theme->horizontal_scroll);
    do_scroll(&scroll.y, scroll_size.height, -dy, theme->vertical_scroll);
}

void View::keep_scroll_within()
{
    on_mouse_wheel(0, 0);
}

void View::on_mouse_pressed(api::MouseButton, const api::Meta&, const Vec2<Dp>&, int)
{
}

void View::on_mouse_moved(const api::Meta&, const Vec2<Dp>&)
{
}

void View::on_mouse_released(api::MouseButton, const api::Meta&, const Vec2<Dp>&)
{
}

void View::on_text(const std::string&)
{
}



///////////////////////////////////////////////////////////////////////////////////////////////////

Rect<Dp> LineView::hit_rect_for_line(const Dp& ypos) const
{
    const auto height = app->Cdp(font->get_height());
    const auto spacing = theme->line_spacing;

    return Rect<Dp>::from_ltrb
    (
        body_rect.get_left(),
        ypos + height + spacing,
        body_rect.get_right(),
        ypos
    );
}


Dp LineView::calculate_line_height() const
{
    return app->Cdp(font->get_height()) + theme->line_spacing;
}


Dp LineView::line_number_to_y(std::size_t line) const
{
    const auto line_offset = static_cast<double>(line + 1) * calculate_line_height();
    return view_rect.height - line_offset;
}


ScrollSize LineView::calculate_scroll_size()
{
    return
    {
        // todo(Gustav): is this correct?
        get_document_width(),
        static_cast<double>(get_number_of_lines()) * calculate_line_height() - calculate_line_height()
    };
}


std::optional<std::size_t> LineView::get_index_under_view_position(const Vec2<Dp> relative_mouse)
{
    const auto p = Vec2<Dp>{relative_mouse.x, relative_mouse.y - scroll.y};

    // todo(Gustav): guesstimate entry from y coordinate and then do the checks to avoid checking all the items...
    for(std::size_t index  = 0; index < get_number_of_lines(); index += 1)
    {
        if(hit_rect_for_line(line_number_to_y(index)).contains(p))
        {
            return index;
        }
    }

    return std::nullopt;
}

void LineView::on_layout_body()
{
    view_rect = body_rect;
}

void LineView::draw_lines(api::Renderer* cache)
{
    // todo(Gustav): guesstimate entry from y coordinate and then do the checks to avoid checking all the items...
    for(std::size_t index = 0; index < get_number_of_lines(); index+=1)
    {
        draw_line(cache, index,
            view_rect.x - scroll.x,
            view_rect.y + line_number_to_y(index) + scroll.y
        );
    }
}

int LineView::absolute_pix_y_to_line(Dp y)
{
    return keep_within
    (
        0,
        static_cast<int>
        (
            std::floor
            (
                ((view_rect.height - y) + view_rect.y + scroll.y) / calculate_line_height()
            )
        ),
        static_cast<int>(get_number_of_lines())
    );
}


}
