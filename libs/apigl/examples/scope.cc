#include "base/color.open-color.h"

#include "apigl/app.h"
#include "apigl/main.h"
#include "apigl/font.h"
#include "apigl/log.h"
#include "api/renderer.h"

using namespace ride::apigl;


struct ScopeApp : App
{
    std::shared_ptr<::Font> font;

    Vec2<pix> mouse = {pix{0}, pix{0}};

    ScopeApp(PlatformArg p)
        : App(p)
        , font(load_font(default_font, 24_px))
    {
    }

    void on_mouse_moved(const Meta&, const Vec2<pix>& new_mouse, pix, pix) override
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
        cache->draw_rect(to_dip(Rect<pix>::from_size(client_size)), colors::white);

        cache->draw_rect(to_dip(Rect<pix>{{50_px, 100_px}, ::Size<pix>{30_px, 30_px}}), colors::red_500);
        cache->draw_rect(to_dip(Rect<pix>{{100_px, 50_px}, ::Size<pix>{30_px, 30_px}}), colors::blue_500);

        auto r = Rect<pix>{{100_px, 100_px}, ::Size<pix>{400_px, 400_px}};

        std::optional<ClipScope> scope_a;

        switch (scope)
        {
        case 0:
            break;
        case 1:
            scope_a = ClipScope{cache, to_dip(r)};
            break;
        case 2:
            scope_a = ClipScope{cache, to_dip(Rect<pix>{mouse, Size<pix>{100_px, 100_px}}.get_offset(Vec2<pix>{-50_px, -50_px}))};
            break;
        default:
            assert(false && "unhandled case");
            break;
        }


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

        cache->draw_text(font, "press space", to_dip(20_px), to_dip(20_px), {0, 0, 0, 255});
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<ScopeApp>(create_platform());});
}

