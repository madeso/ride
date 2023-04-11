#include "api/font.h"
#include "api/renderer.h"

#include "backend/app.h"
#include "backend/main.h"

using namespace ride::backends::gl;

struct ImagesApp : App
{
    Vec2<Dp> mouse = {0_dp,0_dp};

    std::shared_ptr<Font> font;

    ImagesApp(PlatformArg p)
        : App(p)
        , font(load_font(default_font, 24_dp))
    {
    }

    void on_mouse_moved(const Meta&, const Vec2<Dp>& new_mouse, Dp, Dp) override
    {
        mouse = new_mouse;
    }

    void draw(::Renderer* cache) override
    {
        cache->draw_rect(Cpx(Rect<Dp>::from_size(client_size)), Color::rgb(255, 255, 255, 255));

        cache->draw_text(font, "Hello world", Cpx(20_dp), Cpx(20_dp), {0, 0, 0, 255});

        cache->draw_text(font, "mouse", Cpx(mouse.x), Cpx(mouse.y), {0, 0, 255, 255});
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<ImagesApp>(create_platform());});
}

