#pragma once

#include <vector>
#include <memory>

#include "base/vec2.h"
#include "base/size.h"
#include "base/rect.h"
#include "base/cursor.h"
#include "base/units.h"

#include "apigl/key.h"
//#include "apigl/font.h"
#include "apigl/mouse_button.h"

struct Renderer;
struct Font;

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
    virtual void on_mouse_pressed(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse, int clicks);
    virtual void on_mouse_released(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse);
    virtual void on_mouse_moved(const Meta& meta, const vec2<pix>& new_mouse, pix xrel, pix yrel);
    virtual void on_mouse_wheel(int x, int y);
    virtual void on_quit();
    virtual void update();

    // todo(Gustav): wrap Renderer to take virtual sizes to make the usage prettier
    virtual void draw(Renderer* cache) = 0;

    std::shared_ptr<Font> load_font(const std::string_view& file, pix size);
    std::shared_ptr<Font> load_font(const std::string& file, pix size);

    dip to_dip(pix p) const;
    rect<dip> to_dip(const rect<pix>& p) const;
    pix to_pix(dip p) const;
    void set_scale(double d);

    int blink_timer = 0;
    bool run = true;

    size<pix> client_size;
    double scale = 1.0;
    cursor_type cursor = cursor_type::arrow;
    bool* redraw_value;

    std::vector<std::shared_ptr<Font>> loaded_fonts;
};