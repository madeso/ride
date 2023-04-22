#include "core/color.open-color.h"

#include "api/renderer.h"
#include "backend/app.h"
#include "backend/main.h"
#include "backend/log.h"

using namespace ride;
using namespace ride::api;


struct SquaresApp : App
{
    Vec2<Dp> mouse = {Dp{0}, Dp{0}};

    SquaresApp(PlatformArg p)
        : App(p)
    {
    }

    void on_mouse_moved(const Meta&, const Vec2<Dp>& new_mouse, Dp, Dp) override
    {
        mouse = new_mouse;
    }

    void draw(::Renderer* cache) override
    {
        cache->draw_rect(Cpx(Rect<Dp>::from_size(client_size)), colors::white);

        cache->draw_rect(Cpx(Rect<Dp>{{50.0_dp, 100.0_dp}, ::Size<Dp>{30.0_dp, 30.0_dp}}), colors::red_500);
        cache->draw_rect(Cpx(Rect<Dp>{{100.0_dp, 50.0_dp}, ::Size<Dp>{30.0_dp, 30.0_dp}}), colors::blue_500);

        auto r = Rect<Dp>{{100.0_dp, 100.0_dp}, ::Size<Dp>{400.0_dp, 400.0_dp}};

        const auto size = Dp{20};

        {
            auto title = r.cut_top(size);
            for(const auto g: colors::gray)
            {
                cache->draw_rect(Cpx(title.cut_left(size)), g);
            }
            cache->draw_rect(Cpx(title), colors::pink_400);
        }

        {
            auto status = r.cut_bottom(size);
            for(const auto g: colors::cyan)
            {
                cache->draw_rect(Cpx(status.cut_right(size)), g);
            }
            cache->draw_rect(Cpx(status), colors::pink_400);
        }

        cache->draw_rect(Cpx(r), colors::teal_500);

        cache->draw_rect(Cpx(r.get_inset(size)), colors::grape_500);

        cache->draw_rect(Cpx(Rect<Dp>{mouse, ::Size<Dp>{10.0_dp, 10.0_dp}}), colors::black);
    }
};


IMPLMENT_APP(, std::make_unique<SquaresApp>(APP_NAMESPACE::create_platform()))
