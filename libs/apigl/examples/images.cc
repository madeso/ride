#include "api/texture.h"
#include "api/renderer.h"

#include "apigl/app.h"
#include "apigl/main.h"

#include "logo_256text_png.h"

using namespace ride::apigl;

struct ImagesApp : App
{
    Vec2<pix> mouse = {0_px, 0_px};

    std::shared_ptr<Texture> logo;

    ImagesApp(PlatformArg p)
        : App(p)
        , logo(p->load_shared_texture(LOGO_256TEXT_PNG))
    {
    }

    void on_mouse_moved(const Meta&, const Vec2<pix>& new_mouse, pix, pix) override
    {
        mouse = new_mouse;
    }

    void draw(Renderer* cache) override
    {
        cache->draw_rect(to_dip(Rect<pix>::from_size(client_size)), Color::rgb(255, 255, 255, 255));
        cache->draw_rect(to_dip(Rect<pix>{mouse, {10_px, 10_px}}), Color::rgb(0, 0, 255, 255));
        cache->draw_image(logo, to_dip(20_px), to_dip(20_px), {255, 0, 0, 255});
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<ImagesApp>(create_platform());});
}

