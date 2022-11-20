#pragma once

#include <map>

#include "base/cursor.h"

#include "apigl/dependency_sdl.h"

namespace ride::apigl
{

struct cursor_cache
{
    SDL_Cursor* get_or_create(CursorType cursor);
    void set_cursor(CursorType new_cursor);

    std::map<CursorType, SDL_Cursor*> cache;
};

}

