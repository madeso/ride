#pragma once

#include "core/ints.h"

#include "api/mouse_button.h"

namespace ride::backends::sdl
{

api::MouseButton mousebutton_from_sdl_button(i32 btn);

}

