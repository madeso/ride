#include "api/scale.h"

#include <SDL2/SDL.h>

double get_scale()
{
    float dpi;
    SDL_GetDisplayDPI(0, NULL, &dpi, NULL);
#if _WIN32
    return dpi / 96.0;
#else
    return 1.0;
#endif
}
