#include "api/font.h"
#include "api/renderer.h"

#include "apigl/app.h"
#include "apigl/main.h"

using namespace ride::apigl;

struct ImagesApp : App
{
    Vec2<pix> mouse = {0_px,0_px};

    std::shared_ptr<Font> font;

    ImagesApp(PlatformArg p)
        : App(p)
        , font(load_font(default_font, 24_px))
    {
    }

    void on_mouse_moved(const Meta&, const Vec2<pix>& new_mouse, pix, pix) override
    {
        mouse = new_mouse;
    }

    void draw(::Renderer* cache) override
    {
        cache->draw_rect(to_dip(Rect<pix>::from_size(client_size)), Color::rgb(255, 255, 255, 255));

        cache->draw_text(font, "Hello world", to_dip(20_px), to_dip(20_px), {0, 0, 0, 255});

        cache->draw_text(font, "mouse", to_dip(mouse.x), to_dip(mouse.y), {0, 0, 255, 255});
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<ImagesApp>(create_platform());});
}

