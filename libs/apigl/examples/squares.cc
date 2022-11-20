#include "base/color.open-color.h"

#include "api/renderer.h"
#include "apigl/app.h"
#include "apigl/main.h"
#include "apigl/log.h"

using namespace ride::apigl;

struct SquaresApp : App
{
    Vec2<pix> mouse = {pix{0}, pix{0}};

    SquaresApp(PlatformArg p)
        : App(p)
    {
    }

    void on_mouse_moved(const Meta&, const Vec2<pix>& new_mouse, pix, pix) override
    {
        mouse = new_mouse;
    }

    void draw(::Renderer* cache) override
    {
        cache->draw_rect(to_dip(Rect<pix>::from_size(client_size)), colors::white);

        cache->draw_rect(to_dip(Rect<pix>{{50.0_px, 100.0_px}, ::Size<pix>{30.0_px, 30.0_px}}), colors::red_500);
        cache->draw_rect(to_dip(Rect<pix>{{100.0_px, 50.0_px}, ::Size<pix>{30.0_px, 30.0_px}}), colors::blue_500);

        auto r = Rect<pix>{{100.0_px, 100.0_px}, ::Size<pix>{400.0_px, 400.0_px}};

        const auto size = pix{20};

        {
            auto title = r.cut_top(size);
            for(const auto g: colors::gray)
            {
                cache->draw_rect(to_dip(title.cut_left(size)), g);
            }
            cache->draw_rect(to_dip(title), colors::pink_400);
        }

        {
            auto status = r.cut_bottom(size);
            for(const auto g: colors::cyan)
            {
                cache->draw_rect(to_dip(status.cut_right(size)), g);
            }
            cache->draw_rect(to_dip(status), colors::pink_400);
        }

        cache->draw_rect(to_dip(r), colors::teal_500);

        cache->draw_rect(to_dip(r.get_inset(size)), colors::grape_500);

        cache->draw_rect(to_dip(Rect<pix>{mouse, ::Size<pix>{10.0_px, 10.0_px}}), colors::black);
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<SquaresApp>(create_platform());});
}

