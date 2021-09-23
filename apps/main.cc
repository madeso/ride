#include "base/str.h"
#include "base/cc.h"
#include "base/filesystem.h"

#include "api/app.h"
#include "api/rencache.h"
#include "api/image.h"
#include "api/font.h"

#include "ride/document.h"

#include "logo_256text_png.h"

struct Theme
{
    pix line_spacing = pix{3};
    pix gutter_spacing_left = pix{3};
    pix gutter_spacing_right = pix{3};
    pix text_spacing = pix{3};

    pix horizontal_scroll = 21_px;
    pix vertical_scroll = 41_px;

    pix scrollbar_width = pix{12};

    Color window_background_color = {255, 255, 255, 255};
    Color logo_color = {200, 200, 200, 255};
    Color gutter_color = {0, 0, 0, 255};
    Color plain_text_color = {0, 0, 0, 255};

    Color gutter_background = {150, 150, 150, 255};
    Color edit_background = {230, 230, 230, 190};

    Color scroll_through_color = {100, 100, 100, 255};
    Color scroll_thumb_color = {200, 200, 200, 255};
    Color scroll_button_color = {150, 150, 150, 255};
};

struct scroll_size
{
    std::optional<pix> width;
    std::optional<pix> height;
};

struct View
{
    App* app;
    Theme* theme;

    vec2<pix> position =
    {
        pix{0},
        pix{0}
    };

    size<pix> size =
    {
        pix{0},
        pix{0}
    };

    vec2<pix> scroll =
    {
        pix{0},
        pix{0}
    };

    void set_rect(const rect<pix>& r)
    {
        position = {r.x, r.y};
        size = {r.width, r.height};
    }

    void draw_scrollbar
    (
        rect<pix>* view_rect, RenCache* cache,
        pix document_height, pix scroll,
        bool is_vertical, Side through_side, Side top_side, Side bottom_side
    )
    {
        const auto view_rect_width = view_rect->width;
        const auto view_rect_height = view_rect->height;

        const auto size = theme->scrollbar_width;

        const auto view_prop = is_vertical ? view_rect_height : view_rect_width;

        if(view_prop >= document_height)
        {
            return;
        }

        const auto fraction = view_prop / document_height;

        const auto through_rect = view_rect->cut(through_side, size);

        auto track_rect = through_rect;
        const auto top_button_rect = track_rect.cut(top_side, size);
        const auto bottom_button_rect = track_rect.cut(bottom_side, size);

        const auto scroll_fraction = scroll / document_height;

        const auto size_prop = is_vertical ? track_rect.height : track_rect.width;
        const auto thumb_size = size_prop * fraction;
        const auto available_size = size_prop - thumb_size;
        const auto thumb_offset = available_size * scroll_fraction;
        const auto thumb_rect = is_vertical
            ? rect<pix>{track_rect.x               , track_rect.y + thumb_offset, track_rect.width, thumb_size       }
            : rect<pix>{track_rect.x + thumb_offset, track_rect.y               , thumb_size      , track_rect.height}
            ;

        Color scroll_through_color = {100, 100, 100, 255};
        Color scroll_thumb_color = {200, 200, 200, 255};
        Color scroll_button_color = {150, 150, 150, 255};

        const auto clip_scope = ClipScope{cache, app->to_dip(through_rect)};
        cache->draw_rect(app->to_dip(through_rect), theme->scroll_through_color);
        cache->draw_rect(app->to_dip(thumb_rect), theme->scroll_thumb_color);
        cache->draw_rect(app->to_dip(top_button_rect), theme->scroll_button_color);
        cache->draw_rect(app->to_dip(bottom_button_rect), theme->scroll_button_color);
    }

    virtual void draw_body(const rect<pix>& main_view_rect, RenCache* cache) = 0;

    virtual scroll_size calculate_scroll_size() = 0;

    void draw(RenCache* cache)
    {
        auto main_view_rect = rect<pix>{position, size};

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

        draw_body(main_view_rect, cache);
    }

    void on_mouse_wheel(int dx, int dy)
    {
        const auto scroll_size = calculate_scroll_size();

        if(scroll_size.width && dx != 0)
        {
            scroll.x = keep_within
            (
                pix{0},
                scroll.x + static_cast<double>(dx) * theme->horizontal_scroll,
                *scroll_size.width
            );
        }

        if(dy != 0 && scroll_size.height)
        {
            scroll.y = keep_within
            (
                pix{0},
                scroll.y - static_cast<double>(dy) * theme->vertical_scroll,
                *scroll_size.height
            );
        }
    }
};

struct ViewDoc : View
{
    Document doc;
    std::shared_ptr<Font> font;

    pix calculate_line_height()
    {
        return  app->to_pix(font->get_height()) + theme->line_spacing;
    }

    pix get_document_height()
    {
        const auto lines = doc.GetNumberOfLines();
        const auto spacing = calculate_line_height();

        // todo(Gustav): fix cast here
        return spacing * static_cast<double>(lines);
    }

    pix get_document_width()
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

    scroll_size calculate_scroll_size() override
    {
        const auto line_height = calculate_line_height();
        
        const auto w = get_document_width();
        const auto h = get_document_height() - line_height;

        return {w, h};
    }

    void draw_body(const rect<pix>& main_view_rect, RenCache* cache) override
    {
        const auto clip_scope = ClipScope{cache, app->to_dip(main_view_rect)};

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
};


struct RideApp : App
{
    vec2<pix> mouse = {pix{0}, pix{0}};

    std::shared_ptr<Image> logo;
    std::shared_ptr<Font> font;
    std::unique_ptr<filesystem> fs;

    ViewDoc root;
    Theme theme;

    void setup_view(View* view)
    {
        view->theme = &theme;
        view->app = this;
    }

    RideApp()
        : logo(load_shared(LOGO_256TEXT_PNG))
        , font(load_font(Font::default_font, pix{12}))
        , fs(create_local_filesystem())
    {
        setup_view(&root);
        root.font = font;
        root.doc.LoadFile(fs.get(), __FILE__);

        root.position = {pix{50}, pix{50}};
        root.size = {pix{300}, pix{300}};
    }

    void on_mouse_moved(const vec2<pix>& new_mouse, pix xrel, pix yrel) override
    {
        mouse = new_mouse;
    }

    void draw(RenCache* cache) override
    {
        const auto rect = ::rect<pix>::from_size(client_size);
        cache->draw_rect(to_dip(rect), theme.window_background_color);

        cache->draw_image(logo, to_dip(pix{10}), to_dip(pix{10}), theme.logo_color);

        root.set_rect(rect.Inset(pix{20}));

        root.draw(cache);

        cache->draw_rect
        (
            to_dip
            (
                ::rect<pix>
                {
                    mouse,
                    ::size<pix>
                    {
                        pix{10},
                        pix{10}
                    }
                }
            ),
            Color::rgb(0, 0, 255, 255)
        );
    }

    void on_mouse_wheel(int dx, int dy) override
    {
        root.on_mouse_wheel(dx, dy);
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<RideApp>();});
}

