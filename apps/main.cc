#include "base/str.h"
#include "base/cc.h"
#include "base/filesystem.h"

#include "api/app.h"
#include "api/rencache.h"
#include "api/image.h"
#include "api/font.h"

#include "ride/document.h"

#include "logo_256text_png.h"

struct View
{
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

    Document doc;

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
};

struct Theme
{
    pix line_spacing = pix{3};
    pix gutter_spacing_left = pix{3};
    pix gutter_spacing_right = pix{3};
    pix text_spacing = pix{3};

    double lines_to_scroll = 3.0;

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

pix calculate_line_height(const App& app, const Theme& theme, std::shared_ptr<Font> font)
{
    return  app.to_pix(font->get_height()) + theme.line_spacing;
}

pix get_document_height(const Document& doc, const App& app, std::shared_ptr<Font> font, const Theme& theme)
{
    const auto lines = doc.GetNumberOfLines();
    const auto spacing = calculate_line_height(app, theme, font);

    // todo(Gustav): fix cast here
    return spacing * static_cast<double>(lines);
}

pix get_document_width(const Document& doc, const App& app, std::shared_ptr<Font> f)
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

    return app.to_pix(f->get_width(*longest_line));
}

void draw_scrollbar
(
    const View& view, const App& app, const Theme& theme, RenCache* cache,
    std::shared_ptr<Font> font, pix document_height, pix scroll,
    bool is_vertical, Side through_side, Side top_side, Side bottom_side
)
{
    const auto view_rect = rect<pix>{view.position, view.size};
    const auto size = theme.scrollbar_width;

    const auto through_rect = view_rect.get_cut(through_side, size);

    auto track_rect = through_rect;
    const auto top_button_rect = track_rect.cut(top_side, size);
    const auto bottom_button_rect = track_rect.cut(bottom_side, size);

    const auto fraction = (is_vertical ? view_rect.height : view_rect.width) / document_height;

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

    cache->draw_rect(app.to_dip(through_rect), theme.scroll_through_color);
    cache->draw_rect(app.to_dip(thumb_rect), theme.scroll_thumb_color);
    cache->draw_rect(app.to_dip(top_button_rect), theme.scroll_button_color);
    cache->draw_rect(app.to_dip(bottom_button_rect), theme.scroll_button_color);
}

void draw(const View& view, const App& app, const Theme& theme, RenCache* cache, std::shared_ptr<Font> font)
{
    const auto main_view_rect = rect<pix>{view.position, view.size};
    const auto clip_scope = ClipScope{cache, app.to_dip(main_view_rect)};

    const auto lines = view.doc.GetNumberOfLines();

    const auto min_gutter_width = app.to_pix(font->get_width( (Str{} << (lines+1)).ToString().c_str() ));
    const auto gutter_width = min_gutter_width + theme.gutter_spacing_left + theme.gutter_spacing_right;

    auto view_rect = main_view_rect;
    const auto gutter_rect = view_rect.cut_left(gutter_width);

    const auto line_height = calculate_line_height(app, theme, font);
    const auto spacing = line_height;

    cache->draw_rect(app.to_dip(main_view_rect), theme.edit_background);
    cache->draw_rect(app.to_dip(gutter_rect), theme.gutter_background);

    for(int line_index=0; line_index<lines; line_index += 1)
    {
        const auto y = static_cast<double>(line_index) * spacing - view.scroll.y;

        cache->draw_text
        (
            font,
            (Str{} << line_index+1).ToString(),
            app.to_dip(view.position.x + theme.gutter_spacing_left),
            app.to_dip(view.position.y + y),
            theme.gutter_color
        );

        {
            const auto text_scope = ClipScope(cache, app.to_dip(view_rect));
            cache->draw_text
            (
                font,
                view.doc.GetLineAt(line_index),
                app.to_dip(view.position.x + gutter_width + theme.text_spacing - view.scroll.x),
                app.to_dip(view.position.y + y),
                theme.plain_text_color
            );
        }
    }

    
    draw_scrollbar
    (
        view, app, theme, cache, font,
        get_document_height(view.doc, app, font, theme) - line_height, view.scroll.y,
        true, Side::right, Side::top, Side::bottom
    );
    draw_scrollbar
    (
        view, app, theme, cache, font,
        get_document_width(view.doc, app, font), view.scroll.x,
        false, Side::bottom, Side::left, Side::right
    );
}

struct RideApp : App
{
    vec2<pix> mouse = {pix{0}, pix{0}};

    std::shared_ptr<Image> logo;
    std::shared_ptr<Font> font;
    std::unique_ptr<filesystem> fs;

    RideApp()
        : logo(load_shared(LOGO_256TEXT_PNG))
        , font(load_font(Font::default_font, pix{12}))
        , fs(create_local_filesystem())
    {
        root.doc.LoadFile(fs.get(), __FILE__);

        root.position = {pix{50}, pix{50}};
        root.size = {pix{300}, pix{300}};
    }

    void on_mouse_moved(const vec2<pix>& new_mouse, pix xrel, pix yrel) override
    {
        mouse = new_mouse;
    }

    View root;
    Theme theme;

    void draw(RenCache* cache) override
    {
        const auto rect = ::rect<pix>::from_size(client_size);
        cache->draw_rect(to_dip(rect), theme.window_background_color);

        cache->draw_image(logo, to_dip(pix{10}), to_dip(pix{10}), theme.logo_color);

        root.set_rect(rect.Inset(pix{20}));

        ::draw(root, *this, theme, cache, font);

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
        const auto line_height = calculate_line_height(*this, theme, font);

        root.scroll.x = keep_within
        (
            pix{0},
            root.scroll.x + pix{dx*20.0},
            pix{std::numeric_limits<int>::max()}
        );

        const auto old = root.scroll.y;
        root.scroll.y = keep_within
        (
            pix{0},
            root.scroll.y - (static_cast<double>(dy) * line_height) * theme.lines_to_scroll,
            get_document_height
            (
                root.doc, *this, font, theme
            ) - line_height
        );
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<RideApp>();});
}

