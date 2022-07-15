#include "api/app.h"
#include "api/font.h"
#include "api/main.h"

struct ImagesApp : App
{
    vec2<pix> mouse = {0_px,0_px};

    std::shared_ptr<Font> font;

    ImagesApp()
        : font(load_font(Font::default_font, 24_px))
    {
    }

    void on_mouse_moved(const Meta&, const vec2<pix>& new_mouse, pix, pix) override
    {
        mouse = new_mouse;
    }

    void draw(Renderer* cache) override
    {
        draw_rect(cache, to_dip(rect<pix>::from_size(client_size)), Color::rgb(255, 255, 255, 255));

        draw_text(cache, font, "Hello world", to_dip(20_px), to_dip(20_px), {0, 0, 0, 255});

        draw_text(cache, font, "mouse", to_dip(mouse.x), to_dip(mouse.y), {0, 0, 255, 255});
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<ImagesApp>();});
}

