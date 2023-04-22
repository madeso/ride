#include "api/mouse_button.h"

#include <cassert>

#include "core/enumstringmap.h"

using namespace ride::api;

namespace
{
    using ButtonStringMap = ride::EnumStringMap<MouseButton>;

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


namespace ride::api
{


std::optional<MouseButton> mousebutton_from_string(const std::string& str)
{
    return global_button_string_map.from_string(str);
}

std::string to_string(MouseButton key)
{
    return global_button_string_map.to_string(key);
}


}

