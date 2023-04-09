#include "ride/view.document.h"

#include <iostream>

#include "core/c.h"
#include "core/cc.h"
#include "core/utf8.h"

#include "api/app.h"
#include "api/renderer.h"

#include "ride/theme.h"

#include "fmt/ostream.h"
using namespace fmt::literals;

ViewDoc::ViewDoc()
{
    cursor = CursorType::ibeam;
}

void ViewDoc::scroll_to_cursor(const Position& p)
{
    // get pixel position of p
    const auto pt = position_to_upper_left_pix(p);
    const auto pb = position_to_lower_right_pix(p);
    const auto top = pt.y - theme->miminal_scroll_offset - view_rect.y;
    const auto bot = pb.y + theme->miminal_scroll_offset - view_rect.y;

    std::cout << "data " << top.value << " " << bot.value << " " << view_rect.height.value << " " << view_rect.y.value << "\n";

    // determine if it's outside of the range
    auto ychange = 0_dp;
    if(top < 0_dp) { std::cout << "top change\n"; ychange -= top; }
    if(bot > view_rect.height) { std::cout << "bot change\n"; ychange -= bot - view_rect.height;}

    // change scroll to fix
    std::cout << "changing scroll " << ychange.value << "\n";
    scroll.y -= ychange;

    keep_scroll_within();
}

Dp ViewDoc::get_relative_pixel_offset(const Position& p)
{
    return offset_to_relative_left_pix(p.line, p.offset);
}

int ViewDoc::get_offset_from_relative_pixel_offset(int line, Dp offset)
{
    return absolute_pix_x_to_offset(line, offset + view_rect.x);
}

Dp ViewDoc::calculate_line_height()
{
    return  app->Cdp(font->get_height()) + theme->line_spacing;
}


Dp ViewDoc::get_full_document_height()
{
    const auto lines = doc->get_number_of_lines();
    const auto spacing = calculate_line_height();

    // todo(Gustav): fix cast here
    return spacing * static_cast<double>(lines);
}


Dp ViewDoc::get_full_document_width() const
{
    const auto size = doc->get_number_of_lines();
    if(size == 0)
    {
        return 0_dp;
    }
    const auto get_length = [this](int index) -> Dp
    {
        return app->Cdp(font->get_width(doc->get_line_at(index)));
    };

    Dp largest = get_length(0);
    for(int i=1; i<size; i+=1)
    {
        const auto l = get_length(i);
        if(l > largest)
        {
            largest = l;
        }
    }

    return largest;
}


void ViewDoc::draw_line(Renderer* cache, std::size_t line_index, const Dp&, const Dp& y)
{
    cache->draw_text
    (
        font,
        "{0: >{1}}"_format(line_index+1, gutter_char_length),
        app->Cpx(gutter_rect.x + theme->gutter_spacing_left),
        app->Cpx(y),
        theme->gutter_color
    );

    {
        const auto text_scope = ClipScope(cache, app->Cpx(view_rect));
        draw_single_line
        (
            cache, Csizet_to_int(line_index),
            {
                // calculate x instead of using passed x to automatically include gutter width...
                view_rect.x + theme->text_spacing - scroll.x,
                y
            }
        );
    }
}

Dp ViewDoc::get_document_width() const
{
    return get_full_document_width();
}

std::size_t ViewDoc::get_number_of_lines() const
{
    return static_cast<std::size_t>(doc->get_number_of_lines());
}



void ViewDoc::draw_single_line
(
    Renderer* cache,
    int line_index,
    const Vec2<Dp>& position
)
{
    const auto font_height = app->Cdp(font->get_height());

    const auto get_col_x_offset = [this, line_index](int offset) -> Dp
    {
        return offset_to_relative_left_pix(line_index, offset);
    };

    // draw line highlight if caret is on this line
    if( theme->highlight_current_line)
    {
        bool highlight = false;
        for(const auto& sel: cursors)
        {
            if(sel.is_selection()==false && sel.a.line == line_index)
            {
                highlight = true;
            }
        }

        if(highlight)
        {
            cache->draw_rect
            (
                app->Cpx
                (
                    Rect<Dp>
                    {
                        {view_rect.x, position.y},
                        {view_rect.width, font_height}
                    }
                ),
                theme->current_line_background
            );
        }
    }

    // draw selection if it overlaps this line
    const auto& text = doc->get_line_at(line_index);
    for(const auto& sel: cursors)
    {
        auto s = sel.as_sorted();
        if( s.a.line <= line_index && line_index <= s.b.line)
        {
            if( s.a.line != line_index) { s.a.offset = 0; }
            if( s.b.line != line_index) { s.b.offset = C(text.length()); }

            const auto x1 = position.x + get_col_x_offset(s.a.offset);
            const auto x2 = position.x + get_col_x_offset(s.b.offset);
            const auto lh = font_height; // include line spacing here?

            cache->draw_rect
            (
                app->Cpx
                (
                    Rect<Dp>
                    {
                        {x1, position.y},
                        {x2 - x1, lh}
                    }
                ),
                theme->selection_background
            );
        }
    }

    // draw the text
    cache->draw_text
    (
        font,
        doc->get_line_at(line_index),
        app->Cpx(position.x),
        app->Cpx(position.y),
        theme->plain_text_color
    );

    // draw caret if it overlaps this line
    for(const auto& sel: cursors)
    {
        if( sel.a.line == line_index && app->blink_timer < theme->half_blink_period)
        {
            const auto lh = font_height;
            const auto x1 = position.x + get_col_x_offset(sel.a.offset);
            cache->draw_rect
            (
                app->Cpx
                (
                    Rect<Dp>
                    {
                        {x1, position.y},
                        {theme->caret_width, lh}
                    }
                ),
                theme->caret_color
            );
        }
    }

    // if(line_index == 0)
    // {
    //     std::cout << "first line(" << text <<") at " << position << ": "<< view_rect << "\n";
    // }
}


void ViewDoc::on_layout_body()
{
    LineView::on_layout_body();

    // layout debug
    view_rect = view_rect.get_inset(30_dp);

    const auto lines = doc->get_number_of_lines();
    const auto max_gutter_text = fmt::format("{}", lines+1);
    const auto min_gutter_width = app->Cdp(font->get_width( max_gutter_text.c_str() ));
    const auto gutter_width = min_gutter_width + theme->gutter_spacing_left + theme->gutter_spacing_right;

    gutter_rect = view_rect.cut(get_gutter_side(*theme), gutter_width);
    gutter_char_length = max_gutter_text.length();
}


void ViewDoc::draw_body(Renderer* cache)
{
    cache->draw_rect(app->Cpx(view_rect), theme->edit_background);
    cache->draw_rect(app->Cpx(gutter_rect), theme->gutter_background);

    draw_lines(cache);

    for(const auto& sel: cursors)
    {
        if(sel.is_selection() == false)
        {
            const auto draw_p = [this, cache](const Vec2<Dp> p)
            {
                cache->draw_rect(app->Cpx(Rect<Dp>{p, {8_dp, 2_dp}
                }), theme->selection_background);
            };

            draw_p(position_to_upper_left_pix(sel.a));
            draw_p(position_to_lower_right_pix(sel.a));
        }
    }

    // const auto line = absolute_pix_y_to_line(last_mouse.y);
    // const auto offset = absolute_pix_x_to_offset(line, last_mouse.x);
    // draw_text(cache, font, "{} {}: {}"_format(line+1, offset, last_mouse), app->Cpx(view_rect.x + 10_dp), app->Cpx(view_rect.y + 10_dp), {0,0,0});
}


Dp ViewDoc::line_to_relative_upper_pix(int line_index)
{
    const auto spacing = calculate_line_height();
    const auto y = static_cast<double>(line_index) * spacing - scroll.y;
    return y;
}

Dp ViewDoc::line_to_relative_lower_pix(int line_index)
{
    return line_to_relative_upper_pix(line_index + 1);
}

// offset_to_relative_left_pix
Dp ViewDoc::offset_to_relative_left_pix(int line_index, int offset)
{
    const auto& text = doc->get_line_at(line_index);
    const auto t = text.substr(0, Cs(offset));
    return app->Cdp(font->get_width(t));
}

Dp ViewDoc::offset_to_relative_right_pix(int line_index, int offset)
{
    const auto next = doc->position_offset({line_index, offset}, 1);
    const auto next_offset = next.line == line_index ? next.offset : offset;
    return offset_to_relative_left_pix(line_index, next_offset);
}


Vec2<Dp> ViewDoc::position_to_upper_left_pix(const Position& p)
{
    return
    {
        view_rect.x + offset_to_relative_left_pix(p.line, p.offset),
        view_rect.y + line_to_relative_upper_pix(p.line)
    };
}

Vec2<Dp> ViewDoc::position_to_lower_right_pix(const Position& p)
{
    return
    {
        view_rect.x + offset_to_relative_right_pix(p.line, p.offset),
        view_rect.y + line_to_relative_lower_pix(p.line)
    };
}



MinMax<int> ViewDoc::get_line_range()
{
    const auto fist_line_fraction = (scroll.y) / calculate_line_height();
    const auto first_visible_line = std::max(0, static_cast<int>(std::floor(fist_line_fraction)));

    const auto last_line_fraction = (view_rect.height + scroll.y) / calculate_line_height();
    const auto last_visible_line = std::max(0, static_cast<int>(std::ceil(last_line_fraction)));

    return {first_visible_line, last_visible_line};
}




int ViewDoc::absolute_pix_x_to_offset(int line, Dp px)
{
    /*
        index       0     1       2     3       4
                    |     |       |     |       |
                    │ ┌─┐ │ ┌───┐ │ ┌─┐ │ ┌───┐ │
        glyph       │ │a│ │ │ b │ │ │c│ │ │ d │ │
                    │ └─┘ │ └───┘ │ └─┘ │ └───┘ │
                    |     |   ^   |     |       |
    */

    const auto text = this->doc->get_line_at(line);
    const auto chars = utf8_chars(text);

    std::string str;
    int index = 0;

    const auto xx = px - view_rect.x - theme->text_spacing + scroll.x;

    Dp last_width = 0_dp;

    for(const auto& ch: chars)
    {
        str += ch;
        const auto new_width = this->app->Cdp(this->font->get_width(str));
        const auto half = (new_width - last_width) / 2.0;
        const auto middle = last_width + half;

        last_width = new_width;

        if(xx <= middle)
        {
            return index;
        }

        index += 1;
    }

    return C(text.size())+1;
}

Position ViewDoc::translate_view_position(const Vec2<Dp>& p)
{
    const auto line = absolute_pix_y_to_line(p.y);
    const auto byte_offset = absolute_pix_x_to_offset(line, p.x);
    return {line, byte_offset};
}

void clear_all_cursors_but_the_last_one(VirtualView* vview)
{
    if(vview->cursors.size() <= 1)
    {
        return;
    }

    vview->cursors.erase
    (
        vview->cursors.begin(),
        std::next
        (
            vview->cursors.begin(),
            C(vview->cursors.size()) - 1
        )
    );
}

bool destroy_cursors(VirtualView* vview, const SortedSelection& p, bool include_last)
{
    const auto can_toggle = vview->cursors.size() > 1;

    const auto last = include_last
        ? vview->cursors.size()
        :
        (
            vview->cursors.size() > 0
                ? Cs(C(vview->cursors.size())-1)
                : vview->cursors.size()
        );

    for(std::size_t cursor_index = 0; cursor_index < last; cursor_index += 1)
    {
        const auto& s = vview->cursors[cursor_index];
        if(are_overlapping(s.as_sorted(), p))
        {
            if(can_toggle)
            {
                vview->cursors.erase(std::next(vview->cursors.begin(), C(cursor_index)));
                return true;
            }
            else
            {
                std::cout << "unable to toggle the last cursor...\n";
                return true;
            }
        }
    }

    return false;
}

void ViewDoc::on_mouse_pressed(MouseButton button, const Meta& meta, const Vec2<Dp>& new_mouse, int)
{
    last_mouse = new_mouse;
    if(button != MouseButton::left) { return; }

    const auto p = translate_view_position(new_mouse);

    if(meta.alt && meta.ctrl == false)
    {
        if(destroy_cursors(this, {p, p}, true))
        {
            return;
        }
        cursors.emplace_back(Selection{p, p});
    }
    else if(meta.ctrl)
    {
        if(meta.alt == false && cursors.size() > 1)
        {
            clear_all_cursors_but_the_last_one(this);
        }
        if(cursors.empty()) { return; }
        cursors.rbegin()->b = p;
    }
    else
    {
        cursors.clear();
        cursors.emplace_back(Selection{p, p});
    }

    dragging = true;
}

void ViewDoc::drag_to(const Meta& meta, const Vec2<Dp>& new_mouse)
{
    if(dragging == false) { return; }

    // todo(Gustav): handle alt meta drag-select
    // if meta.alt is false, clear everything but the last one

    const auto p = translate_view_position(new_mouse);
    if(cursors.empty()) { return; }

    auto& sel = *cursors.rbegin();
    sel.b = p;

    if(meta.alt)
    {
        // todo(Gustav): check if last selection overlaps any old selection and if so, remove them
        destroy_cursors(this, sel.as_sorted(), false);
    }
}

void ViewDoc::on_mouse_moved(const Meta& meta, const Vec2<Dp>& new_mouse)
{
    last_mouse = new_mouse;

    drag_to(meta, new_mouse);

    if(gutter_rect.contains(new_mouse))
    {
        // toggle breakpoint/flag or folding -> hand
        cursor = CursorType::arrow;
    }
    else if(view_rect.contains(new_mouse))
    {
        cursor = CursorType::ibeam;
    }
    else
    {
        cursor = CursorType::arrow;
    }

}


void ViewDoc::on_mouse_released(MouseButton button, const Meta& meta, const Vec2<Dp>& new_mouse)
{
    last_mouse = new_mouse;

    if(button != MouseButton::left) { return; }
    drag_to(meta, new_mouse);
    dragging = false;
}

void ViewDoc::on_text(const std::string& t)
{
    insert_text_at_cursors(t);
}
