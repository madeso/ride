#include "api/texture.h"
#include "api/renderer.h"

#include "backend/app.h"
#include "backend/main.h"

#include "logo_256text_png.h"

using namespace ride::apigl;

struct ImagesApp : App
{
    Vec2<Dp> mouse = {0_dp, 0_dp};

    std::shared_ptr<Texture> logo;

    ImagesApp(PlatformArg p)
        : App(p)
        , logo(p->load_shared_texture(LOGO_256TEXT_PNG))
    {
    }

    void on_mouse_moved(const Meta&, const Vec2<Dp>& new_mouse, Dp, Dp) override
    {
        mouse = new_mouse;
    }

    void draw(Renderer* cache) override
    {
        cache->draw_rect(Cpx(Rect<Dp>::from_size(client_size)), Color::rgb(255, 255, 255, 255));
        cache->draw_rect(Cpx(Rect<Dp>{mouse, {10_dp, 10_dp}}), Color::rgb(0, 0, 255, 255));
        cache->draw_image(logo, Cpx(20_dp), Cpx(20_dp), {255, 0, 0, 255});
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<ImagesApp>(create_platform());});
}

