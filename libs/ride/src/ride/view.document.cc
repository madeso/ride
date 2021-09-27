#include "ride/view.document.h"

#include <iostream>

#include "base/str.h"
#include "base/c.h"
#include "base/cc.h"
#include "base/utf8.h"

#include "api/app.h"
#include "api/rencache.h"

#include "ride/theme.h"


pix ViewDoc::calculate_line_height()
{
    return  app->to_pix(font->get_height()) + theme->line_spacing;
}


pix ViewDoc::get_full_document_height()
{
    const auto lines = doc.GetNumberOfLines();
    const auto spacing = calculate_line_height();

    // todo(Gustav): fix cast here
    return spacing * static_cast<double>(lines);
}


pix ViewDoc::get_full_document_width()
{
    const auto longest_line = std::max_element
    (
        doc.lines.begin(), doc.lines.end(),
        [](const std::string& lhs, const std::string& rhs) -> bool
        {
            return lhs.length() < rhs.length();
        }
    );

    if(longest_line == doc.lines.end())
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


void draw_single_line
(
    RenCache* cache,
    App* app,
    std::shared_ptr<Font> font,
    int line_index,
    Document& doc,
    Theme* theme,
    const rect<pix>& view_rect,
    const vec2<pix>& position
)
{
    const auto font_height = app->to_pix(font->get_height());

    const auto& text = doc.GetLineAt(line_index);

    const auto get_col_x_offset = [&text, font, app](int offset) -> pix
    {
        const auto t = text.substr(0, Cs(offset));
        return app->to_pix(font->get_width(t));
    };

    // draw selection if it overlaps this line
    for(const auto& sel: doc.cursors)
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
    
    // draw line highlight if caret is on this line
    if( theme->highlight_current_line)
    {
        for(const auto& sel: doc.cursors)
        {
            if(sel.is_selection() && sel.a.line == line_index)
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
    }
    
    // draw the text
    cache->draw_text
    (
        font,
        doc.GetLineAt(line_index),
        app->to_dip(position.x),
        app->to_dip(position.y),
        theme->plain_text_color
    );

    // draw caret if it overlaps this line
    for(const auto& sel: doc.cursors)
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
    const auto lines = doc.GetNumberOfLines();
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
    const auto lines = doc.GetNumberOfLines();

    const auto spacing = calculate_line_height();

    cache->draw_rect(app->to_dip(view_rect), theme->edit_background);
    cache->draw_rect(app->to_dip(gutter_rect), theme->gutter_background);

    const auto line_range = get_line_range();

    for(int line_index=line_range.min; line_index<std::min(lines, line_range.max-1); line_index += 1)
    {
        const auto y = static_cast<double>(line_index) * spacing - scroll.y;

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
                cache, app, font, line_index, doc, theme, view_rect,
                {
                    view_rect.x + theme->text_spacing - scroll.x,
                    view_rect.y + y
                }
            );
        }
    }
}


minmax<int> ViewDoc::get_line_range()
{
    const auto fist_line_fraction = (view_rect.y + scroll.y) / calculate_line_height();
    const auto first_visible_line = std::max(0, static_cast<int>(std::floor(fist_line_fraction)));

    const auto last_line_fraction = (view_rect.y + view_rect.height + scroll.y) / calculate_line_height();
    const auto last_visible_line = std::max(0, static_cast<int>(std::ceil(last_line_fraction)));

    return {first_visible_line, last_visible_line};
}


position ViewDoc::translate_view_position(const vec2<pix>& p)
{
    const auto line = keep_within
    (
        0,
        static_cast<int>
        (
            std::floor
            (
                (p.y - view_rect.y + scroll.y) / calculate_line_height()
            )
        ),
        doc.GetNumberOfLines()
    );

    const auto byte_offset = [&, this]()
    {
        const auto chars = utf8_chars(this->doc.GetLineAt(line));
        
        int index = 0;
        int last_index = 0;

        std::string str;

        for(const auto& ch: chars)
        {
            str += ch;
            const auto w = this->app->to_pix(this->font->get_width(str));
            
            if(w >= p.x - view_rect.x + scroll.x)
            {
                return last_index;
            }

            last_index = index;
            index += 1;
        }

        return last_index;
    }();

    return {line, byte_offset};
}


void ViewDoc::on_mouse_pressed(MouseButton button, const Meta&, const vec2<pix>& new_mouse, int)
{
    if(button != MouseButton::left) { return; }

    const auto p = translate_view_position(new_mouse);

    doc.cursors.clear();
    doc.cursors.emplace_back(selection{p, p});

    dragging = true;
}

void ViewDoc::drag_to(const vec2<pix>& new_mouse)
{
    const auto p = translate_view_position(new_mouse);
    if(doc.cursors.empty()) { return; }

    doc.cursors.rbegin()->b = p;
}

void ViewDoc::on_mouse_moved(const vec2<pix>& new_mouse)
{
    if(dragging)
    {
        drag_to(new_mouse);
    }
}


void ViewDoc::on_mouse_released(MouseButton button, const Meta&, const vec2<pix>& new_mouse)
{
    if(button != MouseButton::left) { return; }
    drag_to(new_mouse);
    dragging = false;
}
