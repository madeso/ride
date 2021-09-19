#include "api/app.h"
#include "api/rencache.h"

#include "open-color.h"

struct SquaresApp : App
{
    vec2 mouse = {0,0};

    void on_mouse_moved(const vec2& new_mouse, int xrel, int yrel) override
    {
        mouse = new_mouse;
    }

    void draw(RenCache* cache) override
    {
        cache->draw_rect(Rect::from_size(size), chex(open_color::white));

        cache->draw_rect(Rect{{50, 100}, Size{30, 30}}, chex(open_color::red_500));
        cache->draw_rect(Rect{{100, 50}, Size{30, 30}}, chex(open_color::blue_500));
        
        auto r = Rect{{100, 100}, Size{400, 400}};
        
        const auto size = 20;

        {
            auto title = r.cut_top(size);
            for(const auto g: open_color::gray)
            {
                cache->draw_rect(title.cut_left(size), chex(g));
            }
            cache->draw_rect(title, chex(open_color::pink_400));
        }

        {
            auto status = r.cut_bottom(size);
            for(const auto g: open_color::cyan)
            {
                cache->draw_rect(status.cut_right(size), chex(g));
            }
            cache->draw_rect(status, chex(open_color::pink_400));
        }

        cache->draw_rect(r, chex(open_color::teal_500));

        cache->draw_rect(r.Inset(size), chex(open_color::grape_500));

        cache->draw_rect(Rect{mouse, Size{10, 10}}, chex(open_color::black));
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<SquaresApp>();});
}

