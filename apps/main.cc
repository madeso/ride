#include "api/app.h"
#include "api/rencache.h"
#include "api/image.h"

#include "logo_256text_png.h"

struct RideApp : App
{
    vec2 mouse = {0,0};

    std::shared_ptr<Image> logo;

    RideApp()
        : logo(load_shared(LOGO_256TEXT_PNG))
    {
    }

    void on_mouse_moved(const vec2& new_mouse, int xrel, int yrel) override
    {
        mouse = new_mouse;
    }

    void draw(RenCache* cache) override
    {
        cache->draw_rect(Rect::from_size(size), Color::rgb(255, 255, 255, 255));

        cache->draw_image(logo, 10, 10, {255, 255, 255, 255});

        cache->draw_rect(Rect{mouse, Size{10, 10}}, Color::rgb(0, 0, 255, 255));
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<RideApp>();});
}

