#include "base/str.h"

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

    Document doc;
};

struct Theme
{
    int line_spacing = 3;
    int gutter_spacing_left = 3;
    int gutter_spacing_right = 3;
    int text_spacing = 3;

    Color window_background_color = {255, 255, 255, 255};
    Color logo_color = {200, 200, 200, 255};
    Color gutter_color = {0, 0, 0, 255};
    Color plain_text_color = {0, 0, 0, 255};

    Color gutter_background = {150, 150, 150, 255};
    Color edit_background = {230, 230, 230, 255};
};

void draw(const View& view, const App& app, const Theme& theme, RenCache* cache, std::shared_ptr<Font> font)
{
    const auto view_rect = Rect{view.position, view.size};
    const auto clip_scope = ClipScope{cache, view_rect};


    const auto lines = view.doc.GetNumberOfLines();

    const auto min_gutter_width = font->get_width( (Str{} << (lines+1)).ToString().c_str() );
    const auto gutter_width = min_gutter_width + theme.gutter_spacing_left + theme.gutter_spacing_right;

    const auto gutter_rect = view_rect.CreateWestFromMaxSize(gutter_width);

    const auto spacing = ( font->get_height() + theme.line_spacing ) * app.get_scale();

    cache->draw_rect(view_rect, theme.edit_background);
    cache->draw_rect(gutter_rect, theme.gutter_background);

    for(int line_index=0; line_index<lines; line_index += 1)
    {
        const auto y = line_index * spacing;

        cache->draw_text(font, (Str{} << line_index+1).ToString(), view.position.x + theme.gutter_spacing_left, view.position.y + y, theme.gutter_color);
        cache->draw_text(font, view.doc.GetLineAt(line_index), view.position.x + gutter_width + theme.text_spacing, view.position.y + y, theme.plain_text_color);
    }
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
        cache->draw_rect(Rect::from_size(size), theme.window_background_color);

        cache->draw_image(logo, 10, 10, theme.logo_color);

        ::draw(root, *this, theme, cache, font);

        cache->draw_rect(Rect{mouse, Size{10, 10}}, Color::rgb(0, 0, 255, 255));
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<RideApp>();});
}

