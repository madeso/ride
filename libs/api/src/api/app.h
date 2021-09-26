#pragma once

#include <vector>

#include "base/vec2.h"
#include "base/size.h"

#include "api/key.h"
#include "api/font.h"
#include "api/mouse_button.h"
#include "api/units.h"
#include "base/rect.h"

struct RenCache;

struct App
{
    App();
    virtual ~App() = default;

    void redraw();

    virtual void on_exposed();
    virtual void on_file_dropped(const std::string& file, pix x, pix y);
    virtual void on_resized(pix new_width, pix new_height);
    virtual void on_key_pressed(const Stroke& key);
    virtual void on_key_released(const Stroke& key);
    virtual void on_text_input(const std::string& str);
    virtual void on_mouse_pressed(MouseButton button, pix x, pix y, int clicks);
    virtual void on_mouse_released(MouseButton button, pix x, pix y);
    virtual void on_mouse_moved(const vec2<pix>& new_mouse, pix xrel, pix yrel);
    virtual void on_mouse_wheel(int x, int y);
    virtual void on_quit();
    virtual void update();

    // todo(Gustav): wrap RenCache to take virtual sizes to make the usage prettier
    virtual void draw(RenCache* cache) = 0;

    std::shared_ptr<Font> load_font(const std::string_view& file, pix size);
    std::shared_ptr<Font> load_font(const std::string& file, pix size);

    dip to_dip(pix p) const;
    rect<dip> to_dip(const rect<pix>& p) const;
    pix to_pix(dip p) const;
    void set_scale(double d);


    int blink_timer;
    bool run;
    
    size<pix> client_size;
    double scale;
    bool* redraw_value;

    std::vector<std::shared_ptr<Font>> loaded_fonts;
};
