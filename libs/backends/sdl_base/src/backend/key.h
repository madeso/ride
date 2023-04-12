#pragma once

#include "core/ints.h"

#include "api/key.h"

namespace ride::backends::sdl
{

Key key_from_sdl_keycode(i32 kc);

}

