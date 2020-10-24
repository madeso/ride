#ifndef RIDE_DRIVER_H
#define RIDE_DRIVER_H

#include <memory>
#include <optional>
#include <string>

namespace ride
{
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


    struct Rgb
    {
        int r;
        int g;
        int b;

        constexpr Rgb(int rr, int gg, int bb)
            : r(rr), g(gg), b(bb) {}
    };

    struct Line
    {
        Rgb color;
        int width;

        constexpr Line(const Rgb& c, int w = 1)
            : color(c), width(w) {}
    };

    struct TextSize
    {
        int width;
        int height;
        int descent;
        int external_leading;

        constexpr TextSize(int w, int h, int d, int e)
            : width(w), height(h), descent(d), external_leading(e) {}
    };

    struct vec2
    {
        int x;
        int y;

        constexpr vec2(int xx, int yy)
            : x(xx), y(yy) {}
    };

    vec2 operator+(const vec2& lhs, const vec2& rhs);

    struct Rect
    {
        vec2 position;
        vec2 size;

        constexpr Rect(const vec2& p, const vec2& s)
            : position(p), size(s) {}
    };

    struct Font
    {
        virtual ~Font();
    };

    struct Driver
    {
        virtual ~Driver();

        virtual void Refresh() = 0;

        virtual std::shared_ptr<Font> CreateCodeFont(int pixel_size) = 0;
        virtual std::shared_ptr<Font> CreateUiFont(int pixel_size) = 0;
        virtual TextSize GetSizeOfString(std::shared_ptr<Font> font, const std::string& str) = 0;
        // std::shared_ptr<Font> LoadFont(const FontDescription&);
        // FontDescription BrowseFont();
    };

    struct Painter
    {
        virtual ~Painter();

        virtual void PushClip(const Rect& rect) = 0;
        virtual void PopClip() = 0;
        
        virtual void Rect(const Rect& rect, std::optional<Rgb> fill, std::optional<Line> line_color) = 0;
        virtual void Circle(const vec2& point, int radius, std::optional<Rgb> fill, std::optional<Line> line_color) = 0;
        virtual void Line(const vec2& from, const vec2& to, const Line& line) = 0;
        virtual void Text(std::shared_ptr<Font> font, const std::string& text, const vec2& where, const Rgb& color) = 0;
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
