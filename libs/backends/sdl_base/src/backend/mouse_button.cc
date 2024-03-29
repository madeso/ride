#include "backend/mouse_button.h"

#include "assert/assert.h"

#include "backend/dependency_sdl.h"

namespace ride::backends::sdl
{

api::MouseButton mousebutton_from_sdl_button(i32 button)
{
    using api::MouseButton;

    switch (button)
    {
    case SDL_BUTTON_LEFT: return MouseButton::left;
    case SDL_BUTTON_MIDDLE: return MouseButton::middle;
    case SDL_BUTTON_RIGHT: return MouseButton::right;
    case SDL_BUTTON_X1: return MouseButton::x1;
    case SDL_BUTTON_X2: return MouseButton::x2;
    default:
        DIE("Unknwon mouse button");
        return MouseButton::none;
    }
}

}

