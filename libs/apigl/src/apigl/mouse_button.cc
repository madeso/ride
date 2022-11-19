#include "apigl/mouse_button.h"

#include <cassert>

#include "base/enumstringmap.h"

#include "apigl/dependency_sdl.h"


namespace
{
    using ButtonStringMap = EnumStringMap<MouseButton>;

    ButtonStringMap build_map()
    {
        ButtonStringMap map;

        map.bind(MouseButton::left, "left");
        map.bind(MouseButton::middle, "middle");
        map.bind(MouseButton::right, "right");
        map.bind(MouseButton::x1, "x1");
        map.bind(MouseButton::x2, "x2");

        return map;
    }

    static const ButtonStringMap global_button_string_map = build_map();
}


std::optional<MouseButton> mousebutton_from_string(const std::string& str)
{
    return global_button_string_map.from_string(str);
}

std::string to_string(MouseButton key)
{
    return global_button_string_map.to_string(key);
}

MouseButton mousebutton_from_sdl_button(i32 button)
{
    switch (button)
    {
    case SDL_BUTTON_LEFT: return MouseButton::left;
    case SDL_BUTTON_MIDDLE: return MouseButton::middle;
    case SDL_BUTTON_RIGHT: return MouseButton::right;
    case SDL_BUTTON_X1: return MouseButton::x1;
    case SDL_BUTTON_X2: return MouseButton::x2;
    default:
        assert(false && "Unknwon mouse button");
        return MouseButton::none;
    }
}
