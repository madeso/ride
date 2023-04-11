#pragma once

#include "core/ints.h"

#include "api/mouse_button.h"

namespace ride::backends::gl
{

MouseButton mousebutton_from_wx_button(i32 btn);

}

