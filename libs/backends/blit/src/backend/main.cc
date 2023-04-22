#include "backend/main.h"

#include <iostream>
#include <sstream>

#include "core/c.h"
#include "core/utf8.h"

#include "api/renderer.h"

#include "backend/cursorcache.h"
#include "backend/log.h"
#include "backend/key.h"
#include "backend/mouse_button.h"
#include "backend/font.h"

#include "backend/dependency_glm.h"

#include "backend/renderer.h"
#include "backend/rencache.h"


#ifdef _WIN32
    #include <windows.h>
    #undef min
    #undef max
#elif __linux__
    #include <unistd.h>
#elif __APPLE__
    #include <mach-o/dyld.h>
#endif

namespace ride::backends::blit
{

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
    const auto counter = SDL_GetPerformanceCounter();
    const auto frequency = SDL_GetPerformanceFrequency();
    return static_cast<double>(counter) / static_cast<double>(frequency);
}

struct MetaState
{
    bool lalt = false;
    bool lctrl = false;
    bool lshift = false;
    bool lgui = false;

    bool ralt = false;
    bool rctrl = false;
    bool rshift = false;
    bool rgui = false;

    api::Meta to_meta() const
    {
        return
        {
            lctrl || rctrl,
            lshift || rshift,
            lalt || ralt,
            lgui || rgui
        };
    }
};

void update_meta(MetaState* meta, SDL_Keycode key, bool state)
{
    switch(key)
    {
    case SDLK_LALT: meta->lalt = state; break;
    case SDLK_LCTRL: meta->lctrl = state; break;
    case SDLK_LSHIFT: meta->lshift = state; break;
    case SDLK_LGUI: meta->lgui = state; break;
    case SDLK_RALT: meta->ralt = state; break;
    case SDLK_RCTRL: meta->rctrl = state; break;
    case SDLK_RSHIFT: meta->rshift = state; break;
    case SDLK_RGUI: meta->rgui = state; break;
    }
}


template<typename T>
T parse(const std::string& str)
{
    std::istringstream ss (str);
    T t;
    ss >> t;
    return t;
}



struct RendererWrapper : api::Renderer
{
    RenCache* cache;

    RendererWrapper(RenCache* c)
        : cache(c)
    {
    }

    void push_clip_rect(const Rect<Px>& r) override
    {
        cache->push_clip_rect(r);
    }

    void pop_clip_rect() override
    {
        cache->pop_clip_rect();
    }

    void draw_rect(const Rect<Px>& r, Color c) override
    {
        cache->draw_rect(r, c);
    }

    void draw_image(std::shared_ptr<api::Texture>, const Rect<Px>&, Color, std::optional<Rectf>, api::Submit) override
    {
    }

    void submit_renderer() override
    {
    }
};

void on_event
(
    const SDL_Event& event, api::App* app, int* window_width, int* window_height,
    MetaState& meta, std::optional<CursorType>& last_cursor, sdl::cursor_cache& c_cache,
    SDL_Window* window
)
{
    // LOG_INFO("Got event {}", event.type);
    switch(event.type)
    {
    case SDL_QUIT:
        app->on_quit();
        break;

    case SDL_WINDOWEVENT:
        switch(event.window.event)
        {
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                // lite:
                // on some systems, when alt-tabbing to the window SDL will queue up
                // several KEYDOWN events for the `tab` key; we flush all keydown
                // events on focus so these are discarded
                SDL_FlushEvent(SDL_KEYDOWN);
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                *window_width = event.window.data1;
                *window_height = event.window.data2;
                app->client_size =
                {
                    app->Cdp(Px{*window_width}),
                    app->Cdp(Px{*window_height})
                };
                app->on_resized
                (
                    app->Cdp(Px{*window_width}),
                    app->Cdp(Px{*window_height})
                );
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                app->on_exposed();
                break;
        }
        break;
    case SDL_DROPFILE:
        {
            // todo(Gustav): is this accurate?
            int mx=0; int my=0;
            int wx=0; int wy=0;
            SDL_GetGlobalMouseState(&mx, &my);
            SDL_GetWindowPosition(window, &wx, &wy);
            app->on_file_dropped
            (
                event.drop.file,
                app->Cdp(Px{mx - wx}),
                app->Cdp(Px{my - wy})
            );
            SDL_free(event.drop.file);
        }
        break;
    case SDL_KEYDOWN:
        update_meta(&meta, event.key.keysym.sym, true);
        app->on_key_pressed({sdl::key_from_sdl_keycode(event.key.keysym.sym), meta.to_meta()});
        break;
    case SDL_KEYUP:
        update_meta(&meta, event.key.keysym.sym, false);
        app->on_key_released({sdl::key_from_sdl_keycode(event.key.keysym.sym), meta.to_meta()});
        break;
    case SDL_TEXTINPUT:
        app->on_text_input(event.text.text);
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == 1) { SDL_CaptureMouse(SDL_TRUE); }
        app->on_mouse_pressed
        (
            sdl::mousebutton_from_sdl_button(event.button.button),
            meta.to_meta(),
            {app->Cdp(Px{event.button.x}), app->Cdp(Px{*window_height - event.button.y})},
            event.button.clicks
        );
        break;
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == 1) { SDL_CaptureMouse(SDL_FALSE); }
        app->on_mouse_released
        (
            sdl::mousebutton_from_sdl_button(event.button.button),
            meta.to_meta(),
            {app->Cdp(Px{event.button.x}), app->Cdp(Px{*window_height - event.button.y})}
        );
        break;
    case SDL_MOUSEMOTION:
        {
        const int mx = event.motion.x;
        const int my = *window_height - event.motion.y;
        const int rx = event.motion.xrel;
        const int ry = -event.motion.yrel;

        // LOG_INFO("Mouse moved to {} {}, ({} {})", mx, my, rx, ry);
        app->on_mouse_moved
        (
            meta.to_meta(),
            Vec2<Dp>
            {
                app->Cdp(Px{mx}),
                app->Cdp(Px{my})
            },
            app->Cdp(Px{rx}),
            app->Cdp(Px{ry})
        );
        }
        break;
    case SDL_MOUSEWHEEL:
        {
            const auto scale = event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -1:1;
            app->on_mouse_wheel(event.wheel.x * scale, event.wheel.y * scale);
        }
        break;
    }

    if(last_cursor.has_value() == false || *last_cursor != app->cursor)
    {
        last_cursor = app->cursor;
        c_cache.set_cursor(app->cursor);
    }
}

bool has_focus(SDL_Window* window)
{
    const auto flags = SDL_GetWindowFlags(window);
    return (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
}

void please_sleep(double seconds)
{
    SDL_Delay(static_cast<Uint32>(seconds * 1000));
}

void wait_event(double seconds)
{
    SDL_WaitEventTimeout(nullptr, static_cast<int>(seconds * 1000));
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

bool step(SDL_Window* window, MetaState* meta, Ren* ren, RenCache* cache, api::App* app, bool first,
          int* window_width, int* window_height, std::optional<CursorType>& last_cursor,
          sdl::cursor_cache& cc)
{
    std::optional<vec2i> mouse_movement;
    int xrel = 0; int yrel = 0;

    {
        bool redraw = first;
        app->redraw_value = nullptr;
        auto scoped_redraw = ScopedValue<bool*>{&app->redraw_value, &redraw};

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
                on_event(event, app, window_width, window_height, *meta, last_cursor, cc,
                         window);

            redraw = true;
        }
        app->update();

        if(mouse_movement)
        {
            app->on_mouse_moved
            (
                meta->to_meta(),
                Vec2<Dp>
                {
                    app->Cdp(Px{mouse_movement->x}),
                    app->Cdp(Px{mouse_movement->y})
                },
                app->Cdp(Px{xrel}),
                app->Cdp(Px{yrel})
            );
        }

        if(redraw == false)
        {
            return false;
        }
    }

    cache->begin_frame();

    const auto ss = ren->get_size();
    app->client_size = Size<Dp>
    {
        app->Cdp(Px{ss.width}),
        app->Cdp(Px{ss.height})
    };

    auto wrapper = RendererWrapper{cache};
    app->draw(&wrapper);
    cache->end_frame();

    return true;
}


int run_main(int argc, char** argv, CreateAppFunction create_app)
{
#ifdef _WIN32
    HINSTANCE lib = LoadLibrary("user32.dll");
    if (lib != nullptr)
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

#if SDL_VERSION_ATLEAST(2, 0, 5)
    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
#endif

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    int window_width = static_cast<int>(dm.w * 0.8);
    int window_height = static_cast<int>(dm.h * 0.8);

    std::optional<double> custom_scale;

    enum class ParserState
    {
        first,
        expect_custom_scale
    };

    ParserState state = ParserState::first;
    bool render_debug = true;

    for (int i = 1; i < argc; i += 1)
    {
        const std::string arg = argv[i];
        switch (state)
        {
        case ParserState::first:
                if (arg == "--scale")
                {
                    state = ParserState::expect_custom_scale;
                }
                else if (arg == "--debug")
                {
                    render_debug = !render_debug;
                }
                else
                {
                    std::cerr << "Invalid argument at " << (i + 1) << ": " << arg << "\n";
                    return -1;
                }
                break;
        case ParserState::expect_custom_scale:
                custom_scale = parse<double>(arg);
                state = ParserState::first;
                break;
        default:
                assert(false && "unhandled parser state");
                return -1;
        }
    }

    if (state != ParserState::first)
    {
        assert(false && "Invalid parser state");
    }

    SDL_Window* window = SDL_CreateWindow(
        "Ride", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN);
    if (window == nullptr)
    {
        LOG_ERROR("Failed to create window: {}", SDL_GetError());
        return -1;
    }

    init_window_icon();

    auto app = create_app({});
    app->client_size = {app->Cdp(Px{window_width}), app->Cdp(Px{window_height})};

    if (custom_scale)
    {
        app->set_scale(*custom_scale);
    }

    MetaState meta;
    std::optional<CursorType> last_cursor;
    sdl::cursor_cache c_cache;

    Ren ren;
    ren.init(window);

    RenCache cache{&ren};
    cache.show_debug = render_debug;

    // auto rc = Renderer{&cache};

    LOG_INFO("Launching window...");

    SDL_ShowWindow(window);

    constexpr double config_fps = 60.0;

    bool first = true;

    while (app->run)
    {
        const auto frame_start = get_time();
        const auto did_redraw = step(window, &meta, &ren, &cache, app.get(), first, &window_width, &window_height, last_cursor, c_cache);

        if (last_cursor.has_value() == false || *last_cursor != app->cursor)
        {
                last_cursor = app->cursor;
                c_cache.set_cursor(app->cursor);
        }

        first = false;
        if (!did_redraw && !has_focus(window))
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


#if 0
    auto render = [&]()
    {
        // todo(Gustav): setup 2d rendering for entire viewport (including titlebar)
        app->draw(&rc);
        rc.render.batch.submit();
        glFlush();
        glFinish();
        SDL_GL_SwapWindow(window);
    };

    while (app->run)
    {
        SDL_Event event;
        if(SDL_WaitEvent(&event) == 0)
        {
            LOG_ERROR("Error waiting for event: {} aborting...", SDL_GetError());
            app->run = false;
            break;
        }
        on_event
        (
            event, app.get(), &window_width, &window_height,
            meta, last_cursor, c_cache,
            window
        );

        if (render_for_each_event)
        {
            while (SDL_PollEvent(&event))
            {
                on_event
                (
                    event, app.get(), &window_width, &window_height,
                    meta, last_cursor, c_cache,
                    window
                );
                app->update();
                render();
            }
        }
        else
        {
            while (SDL_PollEvent(&event))
            {
                on_event
                (
                    event, app.get(), &window_width, &window_height,
                    meta, last_cursor, c_cache,
                    window
                );
            }
            app->update();
            render();
        }
    }
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
#endif
}

