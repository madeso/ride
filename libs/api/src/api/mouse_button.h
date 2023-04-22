#pragma once

#include "core/ints.h"

namespace ride::api
{

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


}

