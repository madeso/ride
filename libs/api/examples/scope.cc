#include "core/color.open-color.h"

#include "backend/app.h"
#include "backend/main.h"
#include "backend/font.h"
#include "backend/log.h"
#include "api/renderer.h"

using namespace ride::apigl;


struct ScopeApp : App
{
    std::shared_ptr<::Font> font;

    Vec2<Dp> mouse = {Dp{0}, Dp{0}};

    ScopeApp(PlatformArg p)
        : App(p)
        , font(load_font(default_font, 24_dp))
    {
    }

    void on_mouse_moved(const Meta&, const Vec2<Dp>& new_mouse, Dp, Dp) override
    {
        mouse = new_mouse;
    }

    int scope = 0;

    void on_key_released(const Stroke& key) override
    {
        if(key.key == Key::space)
        {
            scope = (scope + 1) % 3;
        }
    }

    void draw(::Renderer* cache) override
    {
        cache->draw_rect(Cpx(Rect<Dp>::from_size(client_size)), colors::white);

        cache->draw_rect(Cpx(Rect<Dp>{{50_dp, 100_dp}, ::Size<Dp>{30_dp, 30_dp}}), colors::red_500);
        cache->draw_rect(Cpx(Rect<Dp>{{100_dp, 50_dp}, ::Size<Dp>{30_dp, 30_dp}}), colors::blue_500);

        auto r = Rect<Dp>{{100_dp, 100_dp}, ::Size<Dp>{400_dp, 400_dp}};

        std::optional<ClipScope> scope_a;

        switch (scope)
        {
        case 0:
            break;
        case 1:
            scope_a = ClipScope{cache, Cpx(r)};
            break;
        case 2:
            scope_a = ClipScope{cache, Cpx(Rect<Dp>{mouse, Size<Dp>{100_dp, 100_dp}}.get_offset(Vec2<Dp>{-50_dp, -50_dp}))};
            break;
        default:
            assert(false && "unhandled case");
            break;
        }


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

        cache->draw_text(font, "press space", Cpx(20_dp), Cpx(20_dp), {0, 0, 0, 255});
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<ScopeApp>(create_platform());});
}

