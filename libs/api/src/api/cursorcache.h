#pragma once

#include <map>

#include "base/cursor.h"

#include "api/sdl_dependency.h"


struct cursor_cache
{
    SDL_Cursor* get_or_create(cursor_type cursor);
    void set_cursor(cursor_type new_cursor);

    std::map<cursor_type, SDL_Cursor*> cache;
};
