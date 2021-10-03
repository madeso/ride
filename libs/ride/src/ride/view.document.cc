#include "ride/view.document.h"

#include <iostream>

#include "base/str.h"
#include "base/c.h"
#include "base/cc.h"
#include "base/utf8.h"

#include "api/app.h"
#include "api/rencache.h"

#include "ride/theme.h"

ViewDoc::ViewDoc()
{
    cursor = cursor_type::ibeam;
}

void ViewDoc::scroll_to_cursor(const position& p)
{
    // get pixel position of p
    const auto pt = position_to_upper_left_pix(p);
    const auto pb = position_to_lower_right_pix(p);
    const auto top = pt.y - theme->miminal_scroll_offset - view_rect.y;
    const auto bot = pb.y + theme->miminal_scroll_offset - view_rect.y;

    std::cout << "data " << top.value << " " << bot.value << " " << view_rect.height.value << " " << view_rect.y.value << "\n";
    
    // determine if it's outside of the range
    auto ychange = 0_px;
    if(top < 0_px) { std::cout << "top change\n"; ychange -= top; }
    if(bot > view_rect.height) { std::cout << "bot change\n"; ychange -= bot - view_rect.height;}

    // change scroll to fix
    std::cout << "changing scroll " << ychange.value << "\n";
    scroll.y -= ychange;

    keep_scroll_within();
}

pix ViewDoc::get_relative_pixel_offset(const position& p)
{
    return offset_to_relative_left_pix(p.line, p.offset);
}

int ViewDoc::get_offset_from_relative_pixel_offset(int line, pix offset)
{
    return absolute_pix_x_to_offset(line, offset + view_rect.x);
}

pix ViewDoc::calculate_line_height()
{
    return  app->to_pix(font->get_height()) + theme->line_spacing;
}


pix ViewDoc::get_full_document_height()
{
    const auto lines = doc->GetNumberOfLines();
    const auto spacing = calculate_line_height();

    // todo(Gustav): fix cast here
    return spacing * static_cast<double>(lines);
}


pix ViewDoc::get_full_document_width()
{
    const auto longest_line = std::max_element
    (
        doc->lines.begin(), doc->lines.end(),
        [](const std::string& lhs, const std::string& rhs) -> bool
        {
            return lhs.length() < rhs.length();
        }
    );

    if(longest_line == doc->lines.end())
    {
        return 0_px;
    }

    return app->to_pix(font->get_width(*longest_line));
}


scroll_size ViewDoc::calculate_scroll_size()
{
    const auto line_height = calculate_line_height();
    
    const auto w = get_full_document_width();
    const auto h = get_full_document_height() - line_height;

    return {w, h};
}


void ViewDoc::draw_single_line
(
    RenCache* cache,
    int line_index,
    const vec2<pix>& position
)
{
    const auto font_height = app->to_pix(font->get_height());

    const auto get_col_x_offset = [this, line_index](int offset) -> pix
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
                app->to_dip
                (
                    rect<pix>
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
    const auto& text = doc->GetLineAt(line_index);
    for(const auto& sel: cursors)
    {
        auto s = sel.sorted();
        if( s.a.line <= line_index && line_index <= s.b.line)
        {
            if( s.a.line != line_index) { s.a.offset = 0; }
            if( s.b.line != line_index) { s.b.offset = C(text.length()); }

            const auto x1 = position.x + get_col_x_offset(s.a.offset);
            const auto x2 = position.x + get_col_x_offset(s.b.offset);
            const auto lh = font_height; // include line spacing here?

            cache->draw_rect
            (
                app->to_dip
                (
                    rect<pix>
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
        doc->GetLineAt(line_index),
        app->to_dip(position.x),
        app->to_dip(position.y),
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
                app->to_dip
                (
                    rect<pix>
                    {
                        {x1, position.y},
                        {theme->caret_width, lh}
                    }
                ),
                theme->caret_color
            );
        }
    }
}


pix ViewDoc::get_gutter_width()
{
    const auto lines = doc->GetNumberOfLines();
    const auto min_gutter_width = app->to_pix(font->get_width( (Str{} << (lines+1)).ToString().c_str() ));
    const auto gutter_width = min_gutter_width + theme->gutter_spacing_left + theme->gutter_spacing_right;
    return gutter_width;
}


void ViewDoc::on_layout_body()
{
    view_rect = body_rect;
    gutter_rect = view_rect.cut(get_gutter_side(*theme), get_gutter_width());
}


void ViewDoc::draw_body(RenCache* cache)
{
    const auto lines = doc->GetNumberOfLines();

    cache->draw_rect(app->to_dip(view_rect), theme->edit_background);
    cache->draw_rect(app->to_dip(gutter_rect), theme->gutter_background);

    const auto line_range = get_line_range();

    for(int line_index=line_range.min; line_index<std::min(lines, line_range.max); line_index += 1)
    {
        const auto y = line_to_relative_upper_pix(line_index);

        cache->draw_text
        (
            font,
            (Str{} << line_index+1).ToString(),
            app->to_dip(gutter_rect.x + theme->gutter_spacing_left),
            app->to_dip(gutter_rect.y + y),
            theme->gutter_color
        );

        {
            const auto text_scope = ClipScope(cache, app->to_dip(view_rect));
            draw_single_line
            (
                cache, line_index,
                {
                    view_rect.x + theme->text_spacing - scroll.x,
                    view_rect.y + y
                }
            );
        }
    }

    for(const auto& sel: cursors)
    {
        if(sel.is_selection() == false)
        {
            const auto draw_p = [this, cache](const vec2<pix> p)
            {
                cache->draw_rect(app->to_dip(rect<pix>{p, {8_px, 2_px}
                }), theme->selection_background);
            };

            draw_p(position_to_upper_left_pix(sel.a));
            draw_p(position_to_lower_right_pix(sel.a));
        }
    }
}


pix ViewDoc::line_to_relative_upper_pix(int line_index)
{
    const auto spacing = calculate_line_height();
    const auto y = static_cast<double>(line_index) * spacing - scroll.y;
    return y;
}

pix ViewDoc::line_to_relative_lower_pix(int line_index)
{
    return line_to_relative_upper_pix(line_index + 1);
}

// offset_to_relative_left_pix
pix ViewDoc::offset_to_relative_left_pix(int line_index, int offset)
{
    const auto& text = doc->GetLineAt(line_index);
    const auto t = text.substr(0, Cs(offset));
    return app->to_pix(font->get_width(t));
}

pix ViewDoc::offset_to_relative_right_pix(int line_index, int offset)
{
    const auto next = doc->position_offset({line_index, offset}, 1);
    const auto next_offset = next.line == line_index ? next.offset : offset;
    return offset_to_relative_left_pix(line_index, next_offset);
}


vec2<pix> ViewDoc::position_to_upper_left_pix(const position& p)
{
    return
    {
        view_rect.x + offset_to_relative_left_pix(p.line, p.offset),
        view_rect.y + line_to_relative_upper_pix(p.line)
    };
}

vec2<pix> ViewDoc::position_to_lower_right_pix(const position& p)
{
    return
    {
        view_rect.x + offset_to_relative_right_pix(p.line, p.offset),
        view_rect.y + line_to_relative_lower_pix(p.line)
    };
}



minmax<int> ViewDoc::get_line_range()
{
    const auto fist_line_fraction = (scroll.y) / calculate_line_height();
    const auto first_visible_line = std::max(0, static_cast<int>(std::floor(fist_line_fraction)));

    const auto last_line_fraction = (view_rect.height + scroll.y) / calculate_line_height();
    const auto last_visible_line = std::max(0, static_cast<int>(std::ceil(last_line_fraction)));

    return {first_visible_line, last_visible_line};
}


int ViewDoc::absolute_pix_y_to_line(pix y)
{
    return keep_within
    (
        0,
        static_cast<int>
        (
            std::floor
            (
                (y - view_rect.y + scroll.y) / calculate_line_height()
            )
        ),
        doc->GetNumberOfLines()
    );
}

int ViewDoc::absolute_pix_x_to_offset(int line, pix px)
{
    /*
        index       0     1       2     3       4
                    |     |       |     |       |
                    │ ┌─┐ │ ┌───┐ │ ┌─┐ │ ┌───┐ │
        glyph       │ │a│ │ │ b │ │ │c│ │ │ d │ │
                    │ └─┘ │ └───┘ │ └─┘ │ └───┘ │
                    |     |   ^   |     |       |
    */

    const auto text = this->doc->GetLineAt(line);
    const auto chars = utf8_chars(text);
    
    std::string str;
    int index = 0;

    const auto xx = px - view_rect.x - theme->text_spacing + scroll.x;

    pix last_width = 0_px;

    for(const auto& ch: chars)
    {
        str += ch;
        const auto new_width = this->app->to_pix(this->font->get_width(str));
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

position ViewDoc::translate_view_position(const vec2<pix>& p)
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

bool destroy_cursors(VirtualView* vview, const sorted_selection& p, bool include_last)
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
        if(are_overlapping(s.sorted(), p))
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

void ViewDoc::on_mouse_pressed(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse, int)
{
    if(button != MouseButton::left) { return; }

    const auto p = translate_view_position(new_mouse);

    if(meta.alt && meta.ctrl == false)
    {
        if(destroy_cursors(this, {p, p}, true))
        {
            return;
        }
        cursors.emplace_back(selection{p, p});
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
        cursors.emplace_back(selection{p, p});
    }

    dragging = true;
}

void ViewDoc::drag_to(const Meta& meta, const vec2<pix>& new_mouse)
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
        destroy_cursors(this, sel.sorted(), false);
    }
}

void ViewDoc::on_mouse_moved(const Meta& meta, const vec2<pix>& new_mouse)
{
    drag_to(meta, new_mouse);

    if(gutter_rect.contains(new_mouse))
    {
        // toggle breakpoint/flag or folding -> hand
        cursor = cursor_type::arrow;
    }
    else if(view_rect.contains(new_mouse))
    {
        cursor = cursor_type::ibeam;
    }
    else
    {
        cursor = cursor_type::arrow;
    }
    
}


void ViewDoc::on_mouse_released(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse)
{
    if(button != MouseButton::left) { return; }
    drag_to(meta, new_mouse);
    dragging = false;
}

void ViewDoc::on_text(const std::string& t)
{
    insert_text_at_cursors(t);
}
