#ifndef RIDE_DRIVER_H
#define RIDE_DRIVER_H

#include <memory>
#include <optional>
#include <string>

namespace ride
{
    struct Rgb
    {
        int r;
        int g;
        int b;

        Rgb(int rr, int gg, int bb);
    };

    struct Line
    {
        Rgb color;
        int width;

        Line(const Rgb& c, int w = 1);
    };

    struct vec2
    {
        int x;
        int y;

        vec2(int xx, int yy);
    };

    struct Driver
    {
        virtual ~Driver();

        virtual void Refresh() = 0;
    };

    enum class MouseState { Up, Down, DoubleClick };
    enum class MouseButton { Left, Middle, Right, X1, X2 };

    enum class Key
    {
        None,
        Backspace,
        Tab,
        Return,
        Escape,
        Space,
        Delete,
        Start,
        LButton,
        RButton,
        Cancel,
        MButton,
        Clear,
        Shift,
        Alt,
        Control,
        Menu,
        Pause,
        Capital,
        End,
        Home,
        Left,
        Up,
        Right,
        Down,
        Select,
        Print,
        Execute,
        Snapshot,
        Insert,
        Help,
        Numpad0,
        Numpad1,
        Numpad2,
        Numpad3,
        Numpad4,
        Numpad5,
        Numpad6,
        Numpad7,
        Numpad8,
        Numpad9,
        Multiply,
        Add,
        Separator,
        Subtract,
        Decimal,
        Divide,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        Numlock,
        Scroll,
        PageUp,
        PageDown,
        NumpadSpace,
        NumpadEnter,
        NumpadF1,
        NumpadF2,
        NumpadF3,
        NumpadF4,
        NumpadHome,
        NumpadLeft,
        NumpadUp,
        NumpadRight,
        NumpadDown,
        NumpadPageUp,
        NumpadPageDown,
        NumpadEnd,
        NumpadBegin,
        NumpadInsert,
        NumpadDelete,
        NupadEqual,
        NumpadMultiply,
        NumpadAdd,
        NumpadSeparator,
        NumpadSubtract,
        NumpadDecimal,
        NumpadDivide,
        WindowLeft,
        WindowsRight,
        WindowsMenu,
        Special1,
        Special2,
        Special3,
        Special4,
        Special5,
        Special6,
        Special7,
        Special8,
        Special9,
        Special10,
        Special11,
        Special12,
        Special13,
        Special14,
        Special15,
        Special16,
        Special17,
        Special18,
        Special19,
        Special20


    /* values from 33 to 126 are reserved for the standard ASCII characters */

    /* values from 128 to 255 are reserved for ASCII extended characters
       (note that there isn't a single fixed standard for the meaning
       of these values; avoid them in portable apps!) */
       
    // Hardware-specific buttons
    };

    struct Painter
    {
        virtual ~Painter();
        
        virtual void Rect(const vec2& point, const vec2& size, std::optional<Rgb> fill, std::optional<Line> line_color) = 0;
        virtual void Circle(const vec2& point, int radius, std::optional<Rgb> fill, std::optional<Line> line_color) = 0;

        virtual void Line(const vec2& from, const vec2& to, const Line& line) = 0;

        // todo(Gustav): handle font
        virtual void Text(const std::string& text, const vec2& where, const Rgb& color) = 0;
    };
    
    struct App
    {
        virtual ~App();

        virtual void OnPaint(Painter* painter) = 0;
        virtual void OnSize(const vec2& new_size) = 0;
        virtual void OnMouseMoved(const vec2& new_position) = 0;
        virtual void OnMouseLeftWindow() = 0;
        virtual void OnMouseButton(MouseState state, MouseButton button) = 0;
        virtual void OnMouseScroll(float scrol, int lines) = 0;
        virtual bool OnKey(bool down, Key key) = 0;
        virtual void OnChar(const std::string& str) = 0;
    };

    std::shared_ptr<App> CreateApp(std::shared_ptr<Driver> driver);
}

#endif  // RIDE_DRIVER_H
