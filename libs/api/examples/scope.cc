#include "base/color.open-color.h"
#include "api/app.h"
#include "api/main.h"
#include "api/font.h"
#include "api/log.h"


struct ScopeApp : App
{
    std::shared_ptr<Font> font;

    vec2<pix> mouse = {pix{0}, pix{0}};
    
    ScopeApp()
        : font(load_font(Font::default_font, 24_px))
    {
    }

    void on_mouse_moved(const Meta&, const vec2<pix>& new_mouse, pix, pix) override
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

    void draw(Renderer* cache) override
    {
        draw_rect(cache, to_dip(rect<pix>::from_size(client_size)), colors::white);

        draw_rect(cache, to_dip(rect<pix>{{50_px, 100_px}, ::size<pix>{30_px, 30_px}}), colors::red_500);
        draw_rect(cache, to_dip(rect<pix>{{100_px, 50_px}, ::size<pix>{30_px, 30_px}}), colors::blue_500);
        
        auto r = rect<pix>{{100_px, 100_px}, ::size<pix>{400_px, 400_px}};

        std::optional<ClipScope> scope_a;

        switch (scope)
        {
        case 0:
            break;
        case 1:
            scope_a = ClipScope{cache, to_dip(r)};
            break;
        case 2:
            scope_a = ClipScope{cache, to_dip(rect<pix>{mouse, size<pix>{100_px, 100_px}}.Offset(vec2<pix>{-50_px, -50_px}))};
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
                draw_rect(cache, to_dip(title.cut_left(size)), g);
            }
            draw_rect(cache, to_dip(title), colors::pink_400);
        }


        {
            auto status = r.cut_bottom(size);
            for(const auto g: colors::cyan)
            {
                draw_rect(cache, to_dip(status.cut_right(size)), g);
            }
            draw_rect(cache, to_dip(status), colors::pink_400);
        }

        draw_rect(cache, to_dip(r), colors::teal_500);

        draw_rect(cache, to_dip(r.Inset(size)), colors::grape_500);

        draw_text(cache, font, "press space", to_dip(20_px), to_dip(20_px), {0, 0, 0, 255});
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<ScopeApp>();});
}

