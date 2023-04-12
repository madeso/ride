#pragma once

#include <map>

#include "core/cursor.h"

#include "backend/dependency_sdl.h"

namespace ride::backends::sdl
{

struct cursor_cache
{
    SDL_Cursor* get_or_create(CursorType cursor);
    void set_cursor(CursorType new_cursor);

    std::map<CursorType, SDL_Cursor*> cache;
};

}

