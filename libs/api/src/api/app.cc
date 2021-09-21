#include "api/app.h"

#include "SDL.h"

#include "base/vec2.h"

#include "api/renderer.h"
#include "api/rencache.h"
#include "api/font.h"

#include <cassert>
#include <algorithm>
#include <optional>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#elif __linux__
#include <unistd.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#endif

double calculate_scale()
{
    float dpi;
    SDL_GetDisplayDPI(0, NULL, &dpi, NULL);
#if _WIN32
    return dpi / 96.0;
#else
    return 1.0;
#endif
}

App::App()
    : size{pix{0}, pix{0}}
    , run(true)
    , redraw_value(nullptr)
    , scale(calculate_scale())
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

void App::on_file_dropped(const std::string& file, pix x, pix y)
{
}

void App::on_resized(pix new_width, pix new_height)
{
}

void App::on_key_pressed(Key key)
{
}

void App::on_key_released(Key key)
{
}

void App::on_text_input(const std::string& str)
{
}


void App::on_mouse_pressed(MouseButton button, pix x, pix y, int clicks)
{
}

void App::on_mouse_released(MouseButton button, pix x, pix y)
{
}

void App::on_mouse_moved(const vec2<pix>& new_mouse, pix xrel, pix yrel)
{
}

void App::on_mouse_wheel(int x, int y)
{
}

void App::on_quit()
{
    run = false;
}

void App::update()
{
}

dip App::to_dip(pix p) const
{
    return dip{static_cast<dip::type>(p.value * scale)};
}

rect<dip> App::to_dip(const rect<pix>& p) const
{
    return rect<dip>
    {
        to_dip(p.x),
        to_dip(p.y),
        to_dip(p.width),
        to_dip(p.height)
    };
}

pix App::to_pix(dip p) const
{
    return pix{static_cast<pix::type>(p.value / scale)};
}


void App::set_scale(double d)
{
    scale = d;

    for(auto& f: loaded_fonts)
    {
        f->set_size(to_dip(f->unscaled_size));
    }

     // todo(Gustav): invalidate rendercache...
}

std::shared_ptr<Font> App::load_font(const std::string_view& file, pix size)
{
    return load_font(std::string(file), size);
}

std::shared_ptr<Font> App::load_font(const std::string& file, pix size)
{
    auto r = std::make_shared<Font>();
    if(r->load_font(file.c_str(), to_dip(size)))
    {
        loaded_fonts.emplace_back(r);
        r->unscaled_size = size;
        return r;
    }
    else
    {
        return nullptr;
    }
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

bool step(SDL_Window* window, Ren* ren, RenCache* cache, App* app, bool first)
{
    std::optional<vec2i> mouse_movement;
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
                        app->on_resized
                        (
                            app->to_pix(dip{event.window.data1}),
                            app->to_pix(dip{event.window.data2})
                        );
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
                    app->on_file_dropped
                    (
                        event.drop.file,
                        app->to_pix(dip{mx - wx}),
                        app->to_pix(dip{my - wy})
                    );
                    SDL_free(event.drop.file);
                }
                break;
            case SDL_KEYDOWN:
                app->on_key_pressed(key_from_sdl_keycode(event.key.keysym.sym));
                break;
            case SDL_KEYUP:
                app->on_key_released(key_from_sdl_keycode(event.key.keysym.sym));
                break;

            case SDL_TEXTINPUT:
                app->on_text_input(event.text.text);
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == 1) { SDL_CaptureMouse(SDL_TRUE); }
                app->on_mouse_pressed
                (
                    mousebutton_from_sdl_button(event.button.button),
                    app->to_pix(dip{event.button.x}),
                    app->to_pix(dip{event.button.y}),
                    event.button.clicks
                );
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == 1) { SDL_CaptureMouse(SDL_FALSE); }
                app->on_mouse_released
                (
                    mousebutton_from_sdl_button(event.button.button),
                    app->to_pix(dip{event.button.x}),
                    app->to_pix(dip{event.button.y})
                );
                break;

            case SDL_MOUSEMOTION:
                mouse_movement = vec2i{event.motion.x, event.motion.y};
                xrel += event.motion.xrel;
                yrel += event.motion.yrel;
                break;

            case SDL_MOUSEWHEEL:
                {
                    const auto scale = event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -1:1;
                    app->on_mouse_wheel(event.wheel.x * scale, event.wheel.y * scale);
                }
                break;
            }

            redraw = true;
        }
        app->update();

        if(mouse_movement)
        {
            app->on_mouse_moved
            (
                vec2<pix>
                {
                    app->to_pix(dip{mouse_movement->x}),
                    app->to_pix(dip{mouse_movement->y})
                },
                app->to_pix(dip{xrel}),
                app->to_pix(dip{yrel})
            );
        }

        if(redraw == false)
        {
            return false;
        }
    }

    cache->begin_frame();

    const auto ss = ren->get_size();
    app->size = size<pix>
    {
        app->to_pix(dip{ss.width}),
        app->to_pix(dip{ss.height})
    };
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

template<typename T>
T parse(const std::string& str)
{
    std::istringstream ss (str);
    T t;
    ss >> t;
    return t;
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

    bool render_debug = false;
    std::optional<double> custom_scale;

    enum class ParserState
    {
        first, custom_scale
    };

    ParserState state = ParserState::first;

    for(int i=1; i<argc; i+=1)
    {
        const std::string arg = argv[i];
        switch(state)
        {
        case ParserState::first:
            if(arg == "--debug")
            {
                render_debug = true;
            }
            else if(arg == "--scale")
            {
                state = ParserState::custom_scale;
            }
            else
            {
                std::cerr << "Invalid argument at " << (i+1) << ": " << arg << "\n";
                return -1;
            }
            break;
        case ParserState::custom_scale:
            custom_scale = parse<double>(arg);
            state = ParserState::first;
            break;
        default:
            assert(false && "unhandled parser state");
            return -1;
        }
    }

    if(state != ParserState::first)
    {
        assert(false && "Invalid parser state");
    }

    SDL_Window* window = SDL_CreateWindow(
        "Ride", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, initial_width, initial_height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN);
    init_window_icon();

    Ren ren;
    ren.init(window);

    RenCache cache{&ren};

    cache.show_debug = render_debug;

    auto app = create_app({});
    app->size.height = app->to_pix(dip{initial_height});
    app->size.width = app->to_pix(dip{initial_width});

    if(custom_scale)
    {
        app->set_scale(*custom_scale);
    }

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
