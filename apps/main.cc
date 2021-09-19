#include "base/str.h"
#include "base/cc.h"

#include "api/app.h"
#include "api/rencache.h"
#include "api/image.h"
#include "api/font.h"

#include "ride/document.h"

#include "logo_256text_png.h"
#include "font.ttf.h"

struct View
{
    vec2 position = {0,0};
    Size size = {0,0};

    void set_rect(const Rect& r)
    {
        position = {r.x, r.y};
        size = {r.width, r.height};
    }

    Document doc;

    vec2 scroll = {0, 0};
};

struct Theme
{
    int line_spacing = 3;
    int gutter_spacing_left = 3;
    int gutter_spacing_right = 3;
    int text_spacing = 3;

    int lines_to_scroll = 3;

    int scrollbar_width = 12;

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

int get_document_height(const Document& doc, const App& app, const Theme& theme, std::shared_ptr<Font> font)
{
    const auto lines = doc.GetNumberOfLines();
    const auto spacing = ( font->get_height() + theme.line_spacing ) * app.get_scale();

    return spacing * lines;
}

int calculate_line_height(const App& app, const Theme& theme, std::shared_ptr<Font> font)
{
    return ( font->get_height() + theme.line_spacing ) * app.get_scale();
}

void draw_scrollbar(const View& view, const App& app, const Theme& theme, RenCache* cache, std::shared_ptr<Font> font, int document_height, int scroll)
{
    const auto view_rect = Rect{view.position, view.size};
    const auto size = theme.scrollbar_width * app.get_scale();

    const auto through_rect = view_rect.get_cut_right(size);

    auto track_rect = through_rect;
    const auto top_button_rect = track_rect.cut_top(size);
    const auto bottom_button_rect = track_rect.cut_bottom(size);

    const auto fraction = static_cast<float>(view.size.height) / static_cast<float>(document_height);
    const auto thumb_size = static_cast<int>(track_rect.height * fraction);

    const auto available_size = track_rect.height - thumb_size;

    const auto line_height = calculate_line_height(app, theme, font);

    const auto scroll_size = document_height - line_height;
    const auto scroll_fraction = static_cast<float>(scroll) / static_cast<float>(scroll_size);

    const auto thumb_position = track_rect.y + static_cast<int>(available_size * scroll_fraction);

    const auto thumb_rect = Rect{track_rect.x, thumb_position, track_rect.width, thumb_size};

    Color scroll_through_color = {100, 100, 100, 255};
    Color scroll_thumb_color = {200, 200, 200, 255};
    Color scroll_button_color = {150, 150, 150, 255};

    cache->draw_rect(through_rect, theme.scroll_through_color);
    cache->draw_rect(thumb_rect, theme.scroll_thumb_color);
    cache->draw_rect(top_button_rect, theme.scroll_button_color);
    cache->draw_rect(bottom_button_rect, theme.scroll_button_color);
}

void draw(const View& view, const App& app, const Theme& theme, RenCache* cache, std::shared_ptr<Font> font)
{
    const auto main_view_rect = Rect{view.position, view.size};
    const auto clip_scope = ClipScope{cache, main_view_rect};

    const auto lines = view.doc.GetNumberOfLines();

    const auto min_gutter_width = font->get_width( (Str{} << (lines+1)).ToString().c_str() );
    const auto gutter_width = min_gutter_width + (theme.gutter_spacing_left + theme.gutter_spacing_right) * app.get_scale();

    auto view_rect = main_view_rect;
    const auto gutter_rect = view_rect.cut_left(gutter_width);

    const auto spacing = calculate_line_height(app, theme, font);

    cache->draw_rect(main_view_rect, theme.edit_background);
    cache->draw_rect(gutter_rect, theme.gutter_background);

    for(int line_index=0; line_index<lines; line_index += 1)
    {
        const auto y = line_index * spacing - view.scroll.y * app.get_scale();

        cache->draw_text(font, (Str{} << line_index+1).ToString(), view.position.x + theme.gutter_spacing_left * app.get_scale(), view.position.y + y, theme.gutter_color);

        {
            const auto text_scope = ClipScope(cache, view_rect);
            cache->draw_text(font, view.doc.GetLineAt(line_index), view.position.x + gutter_width + theme.text_spacing - view.scroll.x * app.get_scale(), view.position.y + y, theme.plain_text_color);
        }
    }

    draw_scrollbar(view, app, theme, cache, font, get_document_height(view.doc, app, theme, font), view.scroll.y);
}

struct RideApp : App
{
    vec2 mouse = {0,0};

    std::shared_ptr<Image> logo;
    std::shared_ptr<Font> font;

    RideApp()
        : logo(load_shared(LOGO_256TEXT_PNG))
        , font(font_load(FONT_TTF, 12.0f * get_scale()))
    {
        root.doc.LoadFile(__FILE__);

        root.position = {50, 50};
        root.size = {300, 300};
    }

    void on_mouse_moved(const vec2& new_mouse, int xrel, int yrel) override
    {
        mouse = new_mouse;
    }

    View root;
    Theme theme;

    void draw(RenCache* cache) override
    {
        const auto rect = Rect::from_size(size);
        cache->draw_rect(rect, theme.window_background_color);

        cache->draw_image(logo, 10, 10, theme.logo_color);

        root.set_rect(rect.Inset(static_cast<int>(20 * get_scale())));

        ::draw(root, *this, theme, cache, font);

        cache->draw_rect(Rect{mouse, Size{10, 10}}, Color::rgb(0, 0, 255, 255));
    }

    void on_mouse_wheel(int dx, int dy) override
    {
        const auto line_height = calculate_line_height(*this, theme, font);

        root.scroll.x = keep_within(0, root.scroll.x + dx * 20, std::numeric_limits<int>::max());
        root.scroll.y = keep_within
        (
            0,
            root.scroll.y - dy * line_height * theme.lines_to_scroll,
            get_document_height
            (
                root.doc, *this, theme, font
            ) - line_height
        );
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<RideApp>();});
}

