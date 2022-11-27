#include "apiact/cursorcache.h"

#include <cassert>

namespace ride::apigl
{

namespace
{
    SDL_SystemCursor get_sdl_cursor(CursorType cursor)
    {
        switch(cursor)
        {
            case CursorType::arrow: return SDL_SYSTEM_CURSOR_ARROW;
            case CursorType::ibeam: return SDL_SYSTEM_CURSOR_IBEAM;
            case CursorType::size_horizontal: return SDL_SYSTEM_CURSOR_SIZEWE;
            case CursorType::size_vertical: return SDL_SYSTEM_CURSOR_SIZENS;
            case CursorType::hand: return SDL_SYSTEM_CURSOR_HAND;
            default:
                assert(false && "unhandled cursor type");
                return SDL_SYSTEM_CURSOR_ARROW;
        }
    }
}


SDL_Cursor* cursor_cache::get_or_create(CursorType cursor)
{
    auto found = cache.find(cursor);
    if(found != cache.end()) { return found->second; }

    auto* new_cursor = SDL_CreateSystemCursor(get_sdl_cursor(cursor));
    cache.insert({cursor, new_cursor});
    return new_cursor;
}


void cursor_cache::set_cursor(CursorType new_cursor)
{
    SDL_SetCursor(get_or_create(new_cursor));
}

}

