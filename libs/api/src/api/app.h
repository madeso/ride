#pragma once

#include "embed/types.h"

#include "core/vec2.h"
#include "core/size.h"
#include "core/rect.h"
#include "core/cursor.h"
#include "core/units.h"

#include "api/key.h"
#include "api/mouse_button.h"
#include "api/texture.h"

struct Renderer;
struct Font;
struct Image;

struct Platform
{
    Platform() = default;
    virtual ~Platform() = default;

    virtual double get_default_scale() = 0;
    virtual std::shared_ptr<Font> make_font() = 0;

    virtual std::shared_ptr<Texture> load_shared_texture(const embedded_binary& image_binary) = 0;
    virtual std::shared_ptr<Texture> load_texture(const Image& image) const = 0;
};

using PlatformArg = std::shared_ptr<Platform>;

struct App
{
    explicit App(PlatformArg);
    virtual ~App() = default;

    void redraw();

    virtual void on_exposed();
    virtual void on_file_dropped(const std::string& file, Dp x, Dp y);
    virtual void on_resized(Dp new_width, Dp new_height);
    virtual void on_key_pressed(const Stroke& key);
    virtual void on_key_released(const Stroke& key);
    virtual void on_text_input(const std::string& str);
    virtual void on_mouse_pressed(MouseButton button, const Meta& meta, const Vec2<Dp>& new_mouse, int clicks);
    virtual void on_mouse_released(MouseButton button, const Meta& meta, const Vec2<Dp>& new_mouse);
    virtual void on_mouse_moved(const Meta& meta, const Vec2<Dp>& new_mouse, Dp xrel, Dp yrel);
    virtual void on_mouse_wheel(int x, int y);
    virtual void on_quit();
    virtual void update();

    // todo(Gustav): wrap Renderer to take virtual sizes to make the usage prettier
    virtual void draw(Renderer* cache) = 0;

    std::shared_ptr<Font> load_font(const std::string_view& file, Dp size);
    std::shared_ptr<Font> load_font(const std::string& file, Dp size);

    // todo(Gustav): move dips to scale struct
    Px Cpx(Dp p) const;
    Rect<Px> Cpx(const Rect<Dp>& p) const;
    Dp Cdp(Px p) const;
    void set_scale(double d);

    PlatformArg platform;
    int blink_timer = 0;
    bool run = true;
    Size<Dp> client_size;
    double scale = 1.0;
    CursorType cursor = CursorType::arrow;
    bool* redraw_value;
    std::vector<std::shared_ptr<Font>> loaded_fonts;
};

