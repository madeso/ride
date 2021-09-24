#include "api/app.h"
#include "api/rencache.h"
#include "api/image.h"
#include "api/main.h"

#include "logo_256text_png.h"

struct ImagesApp : App
{
    vec2<pix> mouse = {0_px, 0_px};

    std::shared_ptr<Image> logo;

    ImagesApp()
        : logo(load_shared(LOGO_256TEXT_PNG))
    {
    }

    void on_mouse_moved(const vec2<pix>& new_mouse, pix, pix) override
    {
        mouse = new_mouse;
    }

    void draw(RenCache* cache) override
    {
        cache->draw_rect(to_dip(rect<pix>::from_size(client_size)), Color::rgb(255, 255, 255, 255));

        cache->draw_image(logo, to_dip(20_px), to_dip(20_px), {255, 255, 255, 255});

        cache->draw_rect(to_dip(rect<pix>{mouse, {10_px, 10_px}}), Color::rgb(0, 0, 255, 255));
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<ImagesApp>();});
}

