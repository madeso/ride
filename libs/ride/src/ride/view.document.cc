#include "ride/view.document.h"

#include "base/str.h"
#include "base/c.h"

#include "api/app.h"
#include "api/rencache.h"

#include "ride/theme.h"


pix ViewDoc::calculate_line_height()
{
    return  app->to_pix(font->get_height()) + theme->line_spacing;
}


pix ViewDoc::get_document_height()
{
    const auto lines = doc.GetNumberOfLines();
    const auto spacing = calculate_line_height();

    // todo(Gustav): fix cast here
    return spacing * static_cast<double>(lines);
}


pix ViewDoc::get_document_width()
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
    
    const auto w = get_document_width();
    const auto h = get_document_height() - line_height;

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

void ViewDoc::draw_body(const rect<pix>& main_view_rect, RenCache* cache)
{
    const auto lines = doc.GetNumberOfLines();

    const auto min_gutter_width = app->to_pix(font->get_width( (Str{} << (lines+1)).ToString().c_str() ));
    const auto gutter_width = min_gutter_width + theme->gutter_spacing_left + theme->gutter_spacing_right;

    auto view_rect = main_view_rect;
    const auto gutter_rect = view_rect.cut_left(gutter_width);

    const auto spacing = calculate_line_height();

    cache->draw_rect(app->to_dip(main_view_rect), theme->edit_background);
    cache->draw_rect(app->to_dip(gutter_rect), theme->gutter_background);

    for(int line_index=0; line_index<lines; line_index += 1)
    {
        const auto y = static_cast<double>(line_index) * spacing - scroll.y;

        cache->draw_text
        (
            font,
            (Str{} << line_index+1).ToString(),
            app->to_dip(main_view_rect.x + theme->gutter_spacing_left),
            app->to_dip(main_view_rect.y + y),
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
