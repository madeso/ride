#include "SDL.h"

#include "api/renderer.h"
#include "api/rencache.h"
#include "api/app.h"

#include "api/vec2.h"

#include <cassert>
#include <algorithm>
#include <optional>


#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#elif __linux__
#include <unistd.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#endif

App::App()
    : size{0,0}
    , run(true)
    , redraw_value(nullptr)
{
}

void App::redraw()
{
    assert(redraw_value);
    *redraw_value = true;
}

void App::on_exposed()
{
}

void App::on_file_dropped(const std::string& file, int x, int y)
{
}

void App::on_resized(int new_width, int new_height)
{
}

void App::on_key_pressed(int key)
{
}

void App::on_key_released(int key)
{
}

void App::on_text_input(const std::string& str)
{
}


void App::on_mouse_pressed(int button, int x, int y, int clicks)
{
}

void App::on_mouse_released(int button, int x, int y)
{
}

void App::on_mouse_moved(const vec2& new_mouse, int xrel, int yrel)
{
}

void App::on_mouse_wheel(int y)
{
}

void App::on_quit()
{
    run = false;
}

void App::update()
{
}


#if 0
void init_window_icon(void) {
#ifndef _WIN32
#include "../icon.inl"
  (void) icon_rgba_len; /* unused */
  SDL_Surface *surf = SDL_CreateRGBSurfaceFrom(
    icon_rgba, 64, 64,
    32, 64 * 4,
    0x000000ff,
    0x0000ff00,
    0x00ff0000,
    0xff000000);
  SDL_SetWindowIcon(window, surf);
  SDL_FreeSurface(surf);
#endif
}
#else
void init_window_icon()
{
}
#endif

double get_time()
{
    return SDL_GetPerformanceCounter() / static_cast<double>(SDL_GetPerformanceFrequency());
}

template<typename T>
struct ScopedValue
{
    ScopedValue(T* v, T new_value)
        : value(v)
        , old_value(*v)
    {
        *value = new_value;
    }

    ~ScopedValue()
    {
        *value = old_value;
    }


    T* value;
    T old_value;
};

bool PollEvent(SDL_Event* event)
{
    const auto r = SDL_PollEvent(event);

    if(r && event->type == SDL_WINDOWEVENT)
    {
        switch(event->window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
        case SDL_WINDOWEVENT_EXPOSED:
            return r;
        }

        // on some systems, when alt-tabbing to the window SDL will queue up
        // several KEYDOWN events for the `tab` key; we flush all keydown
        // events on focus so these are discarded
        if (event->window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
        {
            SDL_FlushEvent(SDL_KEYDOWN);
        }

        // ignore this event
        return PollEvent(event);
    }

    return r;
}

int key_name(int k)
{
    return k;
}

int button_name(int k)
{
    return k;
}

bool step(SDL_Window* window, Ren* ren, RenCache* cache, App* app, bool first)
{
    std::optional<vec2> mouse_movement;
    int xrel = 0; int yrel = 0;

    {
        bool redraw = first;
        app->redraw_value = nullptr;
        auto scoped_redraw = ScopedValue<bool*>{&app->redraw_value, &redraw};

        SDL_Event event;
        while(PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                app->on_quit();
                break;

            case SDL_WINDOWEVENT:
                switch(event.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                        app->on_resized(event.window.data1, event.window.data2);
                        break;
                    case SDL_WINDOWEVENT_EXPOSED:
                        cache->invalidate();
                        app->on_exposed();
                        break;
                }
                break;
            case SDL_DROPFILE:
                {
                    int mx=0; int my=0;
                    int wx=0; int wy=0;
                    SDL_GetGlobalMouseState(&mx, &my);
                    SDL_GetWindowPosition(window, &wx, &wy);
                    app->on_file_dropped(event.drop.file, mx - wx, my - wy);
                    SDL_free(event.drop.file);
                }
                break;
            case SDL_KEYDOWN:
                app->on_key_pressed(key_name(event.key.keysym.sym));
                break;
            case SDL_KEYUP:
                app->on_key_released(key_name(event.key.keysym.sym));
                break;

            case SDL_TEXTINPUT:
                app->on_text_input(event.text.text);
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == 1) { SDL_CaptureMouse(SDL_TRUE); }
                app->on_mouse_pressed(button_name(event.button.button), event.button.x, event.button.y, event.button.clicks);
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == 1) { SDL_CaptureMouse(SDL_FALSE); }
                app->on_mouse_released(button_name(event.button.button), event.button.x, event.button.y);
                break;

            case SDL_MOUSEMOTION:
                mouse_movement = vec2{event.motion.x, event.motion.y};
                xrel += event.motion.xrel;
                yrel += event.motion.yrel;
                break;

            case SDL_MOUSEWHEEL:
                app->on_mouse_wheel(event.wheel.y);
                break;
            }

            redraw = true;
        }
        app->update();

        if(mouse_movement)
        {
            app->on_mouse_moved(*mouse_movement, xrel, yrel);
        }

        if(redraw == false)
        {
            return false;
        }
    }

    cache->begin_frame();
    app->size = ren->get_size();
    app->draw(cache);
    cache->end_frame();

    return true;
}

bool has_focus(SDL_Window* window)
{
    const auto flags = SDL_GetWindowFlags(window);
    return (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
}

void please_sleep(double seconds)
{
    SDL_Delay(seconds * 1000);
}

void wait_event(double seconds)
{
    SDL_WaitEventTimeout(nullptr, seconds * 1000);
}

int run_main(int argc, char** argv, CreateAppFunction create_app)
{
#ifdef _WIN32
    HINSTANCE lib = LoadLibrary("user32.dll");
    if(lib != nullptr)
    {
        using SetProcessDPIAwareFun = int (*)();
        auto SetProcessDPIAware = (SetProcessDPIAwareFun)GetProcAddress(lib, "SetProcessDPIAware");
        SetProcessDPIAware();
    }
#endif

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }
    SDL_EnableScreenSaver();
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

#ifdef SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR /* Available since 2.0.8 */
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
#endif

#if SDL_VERSION_ATLEAST(2, 0, 5)
    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
#endif

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    const int initial_width = dm.w * 0.8;
    const int initial_height = dm.h * 0.8;

    SDL_Window* window = SDL_CreateWindow(
        "Ride", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, initial_width, initial_height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN);
    init_window_icon();

    Ren ren;
    ren.init(window);

    RenCache cache{&ren};

    cache.show_debug = true;

    auto app = create_app();
    app->size.height = initial_height;
    app->size.width = initial_width;

    constexpr double config_fps = 60.0;

    bool first = true;

    while (app->run)
    {
        const auto frame_start = get_time();
        const auto did_redraw = step(window, &ren, &cache, app.get(), first);
        first = false;
        if(!did_redraw && !has_focus(window))
        {
            wait_event(0.25);
        }
        const auto elapsed = get_time() - frame_start;
        please_sleep(std::max(0.0, (1.0 / config_fps) - elapsed));
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}