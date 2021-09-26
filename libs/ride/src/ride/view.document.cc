#include "ride/view.document.h"

#include "base/str.h"

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
            cache->draw_text
            (
                font,
                doc.GetLineAt(line_index),
                app->to_dip(main_view_rect.x + gutter_width + theme->text_spacing - scroll.x),
                app->to_dip(main_view_rect.y + y),
                theme->plain_text_color
            );
        }
    }
}
