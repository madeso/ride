#include "api/app.h"
#include "api/rencache.h"

struct SquaresApp : App
{
    vec2 mouse = {0,0};

    void on_mouse_moved(const vec2& new_mouse, int xrel, int yrel) override
    {
        mouse = new_mouse;
    }

    void draw(RenCache* cache) override
    {
        cache->draw_rect(Rect::from_size(size), Color::rgb(255, 255, 255, 255));

        cache->draw_rect(Rect{{50, 100}, Size{10, 10}}, Color::rgb(0, 0, 255, 255));
        cache->draw_rect(Rect{{100, 50}, Size{10, 10}}, Color::rgb(255, 0, 0, 255));
        cache->draw_rect(Rect{{100, 100}, Size{100, 100}}, Color::rgb(0, 255, 0, 255));

        cache->draw_rect(Rect{mouse, Size{10, 10}}, Color::rgb(0, 0, 0, 255));
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<SquaresApp>();});
}

