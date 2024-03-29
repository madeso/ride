#include "backend/main.h"

#include <iostream>
#include <sstream>

#include "core/c.h"
#include "core/utf8.h"

#include "api/renderer.h"

#include "backend/cursorcache.h"
#include "backend/log.h"
#include "backend/spritebatch.h"
#include "backend/key.h"
#include "backend/mouse_button.h"
#include "backend/font.h"

#include "backend/dependency_opengl.h"
#include "backend/dependency_glm.h"

#ifdef _WIN32
    #include <windows.h>
    #undef min
    #undef max
#elif __linux__
    #include <unistd.h>
#elif __APPLE__
    #include <mach-o/dyld.h>
#endif



namespace
{
    constexpr bool render_for_each_event = false;
    constexpr bool use_vsynced_rendering = false;
}


namespace ride::backends::gl
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


GLint Cint_to_glint(int i)
{
    return i;
}

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



struct Renderer : api::Renderer
{
    Render2 render;
    std::vector<Rect<Px>> rects;

    void update_stencil()
    {
        Renderer* c = this;

        if(c->rects.empty())
        {
            glDisable(GL_STENCIL_TEST);
        }
        else
        {
            glEnable(GL_STENCIL_TEST);
            glStencilMask(0xFF);

            // clear to 0
            glClearStencil(0);
            glClear(GL_STENCIL_BUFFER_BIT);

            // enable writing, increase when succeeded and write all "scope" rects
            glStencilOp(GL_INCR, GL_INCR, GL_INCR);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            for(const auto& r: c->rects)
            {
                c->render.batch.quad
                (
                    std::nullopt,
                    {
                        Cint_to_float(r.x.value),
                        Cint_to_float(r.y.value),
                        Cint_to_float(r.width.value),
                        Cint_to_float(r.height.value),
                    },
                    std::nullopt,
                    // hack? use alpha=0 to write to the stencil buffer but the not color buffer
                    {255, 255, 255, 0}
                );
            }
            c->render.batch.submit();

            // disable write to stencil buffer, only render where all rects has been drawn
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glStencilFunc(GL_EQUAL, Csizet_to_glsizei(c->rects.size()), 0xFF);
        }
    }

    void push_clip_rect(const Rect<Px>& r) override
    {
        Renderer* c = this;
        c->rects.emplace_back(r);
        update_stencil();
    }

    void pop_clip_rect() override
    {
        Renderer* c = this;
        c->rects.pop_back();
        update_stencil();
    }

    void draw_rect(const Rect<Px>& r, Color c) override
    {
        Renderer* ren = this;
        // LOG_INFO("Drawing rect {} {} {} {}", r.x.value, r.y.value, r.width.value, r.height.value);
        ren->render.batch.quad
        (
            std::nullopt,
            {
                Cint_to_float(r.x.value),
                Cint_to_float(r.y.value),
                Cint_to_float(r.width.value),
                Cint_to_float(r.height.value)
            },
            std::nullopt,
            {c.r/255.0f, c.g/255.0f, c.b/255.0f, c.a/255.0f}
        );

        // submit after each quad?
        // ren->render.batch.submit();
    }

    void draw_image(std::shared_ptr<api::Texture> the_texture, const Rect<Px>& rect, Color c, std::optional<Rectf> sub, api::Submit submit) override
    {
        auto texture = std::static_pointer_cast<Texture>(the_texture);
        Renderer* ren = this;
        // LOG_INFO("Drawing image {} {} {} {}", x.value, y.value, texture->width, texture->height);
        ren->render.batch.quad
        (
            texture.get(),
            {
                Cint_to_float(rect.x.value),
                Cint_to_float(rect.y.value),
                Cint_to_float(rect.width.value),
                Cint_to_float(rect.height.value),
            },
            sub,
            {c.r/255.0f, c.g/255.0f, c.b/255.0f, c.a/255.0f}
        );

        // submit after each quad?
        if(submit == api::Submit::yes)
        {
            // ren->render.batch.submit();
        }
    }

    void submit_renderer() override
    {
        // render.batch.submit();
    }
};


#if 0
// register this somewhere
int filterEvent(void* userdata, SDL_Event* event)
{
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
    {
        // convert userdata pointer to yours and trigger your own draw function
        // this is called very often now
        // IMPORTANT: Might be called from a different thread, see SDL_SetEventFilter docs
        App* app = static_cast<App*>(userdata);
        app->draw();

        // return 0 if you don't want to handle this event twice
        return 0;
    }

    return 1;
}
#endif

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

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#if SDL_VERSION_ATLEAST(2, 0, 5)
    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
#endif

    SDL_GL_SetSwapInterval(use_vsynced_rendering ? 1 : 0);

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    int window_width = static_cast<int>(dm.w * 0.8);
    int window_height = static_cast<int>(dm.h * 0.8);

    std::optional<double> custom_scale;

    enum class ParserState
    {
        first, expect_custom_scale
    };

    ParserState state = ParserState::first;

    for(int i=1; i<argc; i+=1)
    {
        const std::string arg = argv[i];
        switch(state)
        {
        case ParserState::first:
            if(arg == "--scale")
            {
                state = ParserState::expect_custom_scale;
            }
            else
            {
                std::cerr << "Invalid argument at " << (i+1) << ": " << arg << "\n";
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

    if(state != ParserState::first)
    {
        assert(false && "Invalid parser state");
    }

    SDL_Window* window = SDL_CreateWindow
    (
        "Ride",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_width, window_height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL
    );
    if(window == nullptr)
    {
        LOG_ERROR("Failed to create window: {}", SDL_GetError());
        return -1;
    }

    init_window_icon();

    SDL_GLContext glcontext = SDL_GL_CreateContext(window);

    if(glcontext == nullptr)
    {
        LOG_ERROR("Could not create window: {}", SDL_GetError());

        SDL_DestroyWindow(window);
        return -1;
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        LOG_ERROR("Failed to initialize OpenGL context");

        SDL_GL_DeleteContext(glcontext);
        SDL_DestroyWindow(window);
        return -1;
    }

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto app = create_app({});
    app->client_size =
    {
        app->Cdp(Px{window_width}),
        app->Cdp(Px{window_height})
    };

    if(custom_scale)
    {
        app->set_scale(*custom_scale);
    }

    MetaState meta;
    std::optional<CursorType> last_cursor;
    sdl::cursor_cache c_cache;

    Renderer rc;

    LOG_INFO("Launching window...");

    SDL_ShowWindow(window);

    const auto error = SDL_GL_MakeCurrent(window, glcontext);
    if (error != 0)
    {
        LOG_ERROR("Unable to make current");
    }

    auto render = [&]()
    {
        // todo(Gustav): setup 2d rendering for entire viewport (including titlebar)
        {
            glViewport(0, 0, Cint_to_glint(window_width), Cint_to_glint(window_height));

            const auto camera = glm::mat4(1.0f);
            const auto projection =
                glm::ortho(0.0f, Cint_to_float(window_width), 0.0f, Cint_to_float(window_height));
            rc.render.quad_shader.use();
            rc.render.quad_shader.set_mat(rc.render.view_projection_uniform, projection);
            rc.render.quad_shader.set_mat(rc.render.transform_uniform, camera);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

}

