#include "SDL.h"

#include "ride/driver.h"
#include "api/renderer.h"
#include "api/rencache.h"

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#endif

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

int main(int argc, char** argv)
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

    SDL_Window* window = SDL_CreateWindow(
        "Ride", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, dm.w * 0.8, dm.h * 0.8,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN);
    init_window_icon();

    Ren ren;
    ren.init(window);

    RenCache cache{&ren};

    cache.show_debug = true;

    while (true)
    {
        SDL_Event event;
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
        {
            break;
        }

        cache.begin_frame();
        auto size = ren.get_size();
        cache.draw_rect(Rect::from_size(size), Color::rgb(255, 255, 255, 255));
        cache.end_frame();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
