#pragma once

#include <functional>
#include <memory>

#include "SDL.h"

#include "base/vec2.h"
#include "base/size.h"

#include "api/key.h"
#include "api/mouse_button.h"

struct RenCache;

struct App
{
    App();
    virtual ~App() = default;

    void redraw();

    virtual void on_exposed();
    virtual void on_file_dropped(const std::string& file, int x, int y);
    virtual void on_resized(int new_width, int new_height);

    virtual void on_key_pressed(Key key);
    virtual void on_key_released(Key key);
    virtual void on_text_input(const std::string& str);

    virtual void on_mouse_pressed(MouseButton button, int x, int y, int clicks);
    virtual void on_mouse_released(MouseButton button, int x, int y);
    virtual void on_mouse_moved(const vec2& new_mouse, int xrel, int yrel);
    virtual void on_mouse_wheel(int x, int y);

    virtual void on_quit();

    virtual void update();
    virtual void draw(RenCache* cache) = 0;

    Size size;
    bool run;

    double get_scale() const;

    bool* redraw_value;
};

struct StartupArguments
{
};

using CreateAppFunction = std::function<std::unique_ptr<App> (const StartupArguments&)>;
int run_main(int argc, char** argv, CreateAppFunction create_app);
