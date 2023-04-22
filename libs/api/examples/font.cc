#include "api/font.h"
#include "api/renderer.h"

#include "backend/app.h"
#include "backend/main.h"


using namespace ride;
using namespace ride::api;

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

        font->draw(cache, "Hello world", Cpx(20_dp), Cpx(20_dp), {0, 0, 0, 255});
        font->draw(cache, "mouse", Cpx(mouse.x), Cpx(mouse.y), {0, 0, 255, 255});
    }
};


IMPLMENT_APP(, std::make_unique<ImagesApp>(APP_NAMESPACE::create_platform()))
