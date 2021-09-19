#pragma once

#include <string>
#include <optional>

#include "base/ints.h"

enum class MouseButton
{
    none,
    left,
    middle,
    right,
    x1,
    x2
};

std::optional<MouseButton> mousebutton_from_string(const std::string& str);
std::string to_string(MouseButton key);
MouseButton mousebutton_from_sdl_button(i32 btn);
