#include "api/app.h"
#include "api/rencache.h"
#include "api/main.h"

#include "open-color.h"

struct SquaresApp : App
{
    vec2<pix> mouse = {pix{0}, pix{0}};

    void on_mouse_moved(const vec2<pix>& new_mouse, pix, pix) override
    {
        mouse = new_mouse;
    }

    void draw(RenCache* cache) override
    {
        cache->draw_rect(to_dip(rect<pix>::from_size(client_size)), chex(open_color::white));

        cache->draw_rect(to_dip(rect<pix>{{50.0_px, 100.0_px}, ::size<pix>{30.0_px, 30.0_px}}), chex(open_color::red_500));
        cache->draw_rect(to_dip(rect<pix>{{100.0_px, 50.0_px}, ::size<pix>{30.0_px, 30.0_px}}), chex(open_color::blue_500));
        
        auto r = rect<pix>{{100.0_px, 100.0_px}, ::size<pix>{400.0_px, 400.0_px}};
        
        const auto size = pix{20};

        {
            auto title = r.cut_top(size);
            for(const auto g: open_color::gray)
            {
                cache->draw_rect(to_dip(title.cut_left(size)), chex(g));
            }
            cache->draw_rect(to_dip(title), chex(open_color::pink_400));
        }

        {
            auto status = r.cut_bottom(size);
            for(const auto g: open_color::cyan)
            {
                cache->draw_rect(to_dip(status.cut_right(size)), chex(g));
            }
            cache->draw_rect(to_dip(status), chex(open_color::pink_400));
        }

        cache->draw_rect(to_dip(r), chex(open_color::teal_500));

        cache->draw_rect(to_dip(r.Inset(size)), chex(open_color::grape_500));

        cache->draw_rect(to_dip(rect<pix>{mouse, ::size<pix>{10.0_px, 10.0_px}}), chex(open_color::black));
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<SquaresApp>();});
}

