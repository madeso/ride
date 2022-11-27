#pragma once

#include "base/ints.h"

#include "api/mouse_button.h"

namespace ride::apigl
{

MouseButton mousebutton_from_sdl_button(i32 btn);

}

