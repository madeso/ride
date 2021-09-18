#include "api/app.h"
#include "api/rencache.h"
#include "api/font.h"

#include "font.ttf.h"

struct ImagesApp : App
{
    vec2 mouse = {0,0};

    std::shared_ptr<Font> font;

    ImagesApp()
        : font(font_load(FONT_TTF, 12.0f * get_scale()))
    {
    }

    void on_mouse_moved(const vec2& new_mouse, int xrel, int yrel) override
    {
        mouse = new_mouse;
    }

    void draw(RenCache* cache) override
    {
        cache->draw_rect(Rect::from_size(size), Color::rgb(255, 255, 255, 255));

        cache->draw_text(font, "Hello world", 20, 20, {0, 0, 0, 255});

        cache->draw_text(font, "mouse", mouse.x, mouse.y, {0, 0, 255, 255});
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<ImagesApp>();});
}

