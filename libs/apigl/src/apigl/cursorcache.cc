#include "apigl/cursorcache.h"

#include <cassert>


namespace
{
    SDL_SystemCursor get_sdl_cursor(cursor_type cursor)
    {
        switch(cursor)
        {
            case cursor_type::arrow: return SDL_SYSTEM_CURSOR_ARROW;
            case cursor_type::ibeam: return SDL_SYSTEM_CURSOR_IBEAM;
            case cursor_type::size_horizontal: return SDL_SYSTEM_CURSOR_SIZEWE;
            case cursor_type::size_vertical: return SDL_SYSTEM_CURSOR_SIZENS;
            case cursor_type::hand: return SDL_SYSTEM_CURSOR_HAND;
            default:
                assert(false && "unhandled cursor type");
                return SDL_SYSTEM_CURSOR_ARROW;
        }
    }
}


SDL_Cursor* cursor_cache::get_or_create(cursor_type cursor)
{
    auto found = cache.find(cursor);
    if(found != cache.end()) { return found->second; }

    auto* new_cursor = SDL_CreateSystemCursor(get_sdl_cursor(cursor));
    cache.insert({cursor, new_cursor});
    return new_cursor;
}


void cursor_cache::set_cursor(cursor_type new_cursor)
{
    SDL_SetCursor(get_or_create(new_cursor));
}
