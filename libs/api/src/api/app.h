#pragma once

#include <vector>
#include <memory>

#include "embed/types.h"

#include "base/vec2.h"
#include "base/size.h"
#include "base/rect.h"
#include "base/cursor.h"
#include "base/units.h"

#include "api/key.h"
#include "api/mouse_button.h"
#include "api/texture.h"

struct Renderer;
struct Font;

struct Platform
{
    Platform() = default;
    virtual ~Platform() = default;

    virtual double get_default_scale() = 0;
    virtual std::shared_ptr<Font> make_font() = 0;

    virtual std::shared_ptr<Texture> load_shared_texture(const embedded_binary& image_binary) = 0;
};

using PlatformArg = std::shared_ptr<Platform>;

struct App
{
    explicit App(PlatformArg);
    virtual ~App() = default;

    void redraw();

    virtual void on_exposed();
    virtual void on_file_dropped(const std::string& file, pix x, pix y);
    virtual void on_resized(pix new_width, pix new_height);
    virtual void on_key_pressed(const Stroke& key);
    virtual void on_key_released(const Stroke& key);
    virtual void on_text_input(const std::string& str);
    virtual void on_mouse_pressed(MouseButton button, const Meta& meta, const Vec2<pix>& new_mouse, int clicks);
    virtual void on_mouse_released(MouseButton button, const Meta& meta, const Vec2<pix>& new_mouse);
    virtual void on_mouse_moved(const Meta& meta, const Vec2<pix>& new_mouse, pix xrel, pix yrel);
    virtual void on_mouse_wheel(int x, int y);
    virtual void on_quit();
    virtual void update();

    // todo(Gustav): wrap Renderer to take virtual sizes to make the usage prettier
    virtual void draw(Renderer* cache) = 0;

    std::shared_ptr<Font> load_font(const std::string_view& file, pix size);
    std::shared_ptr<Font> load_font(const std::string& file, pix size);

    // todo(Gustav): move dips to scale struct
    dip to_dip(pix p) const;
    Rect<dip> to_dip(const Rect<pix>& p) const;
    pix to_pix(dip p) const;
    void set_scale(double d);

    PlatformArg platform;
    int blink_timer = 0;
    bool run = true;
    Size<pix> client_size;
    double scale = 1.0;
    CursorType cursor = CursorType::arrow;
    bool* redraw_value;
    std::vector<std::shared_ptr<Font>> loaded_fonts;
};

