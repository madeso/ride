#pragma once

#include <map>

#include "base/cursor.h"

#include "apigl/dependency_sdl.h"

namespace ride::apigl
{

struct cursor_cache
{
    SDL_Cursor* get_or_create(cursor_type cursor);
    void set_cursor(cursor_type new_cursor);

    std::map<cursor_type, SDL_Cursor*> cache;
};

}

