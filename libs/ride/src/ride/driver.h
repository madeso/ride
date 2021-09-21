#ifndef RIDE_DRIVER_H
#define RIDE_DRIVER_H

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace ride
{
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

    template<typename T>
    T& operator<<(T& t, const vec2& v)
    {
        t << "(" << v.x << ", " << v.y << ")";
        return t;
    }

    struct Arguments
    {
        std::string name;
        std::vector<std::string> arguments;
    };

    struct Font
    {
        virtual ~Font();

        // only for fixed size?
        int line_height = 0;
        int char_width = 0;
    };

    struct Driver
    {
        virtual ~Driver();

        virtual void Refresh() = 0;

        virtual std::shared_ptr<Font> CreateCodeFont(int pixel_size) = 0;
        virtual std::shared_ptr<Font> CreateUiFont(int pixel_size) = 0;
        // std::shared_ptr<Font> LoadFont(const FontDescription&);
        // FontDescription BrowseFont();

        virtual TextSize GetSizeOfString(std::shared_ptr<Font> font, const std::string& str) = 0;
    };

    struct Painter
    {
        virtual ~Painter();

        virtual void PushClip(const recti& rect) = 0;
        virtual void PopClip() = 0;
        
        virtual void recti(const recti& rect, std::optional<Rgb> fill, std::optional<Line> line_color) = 0;
        virtual void Circle(const vec2& point, int radius, std::optional<Rgb> fill, std::optional<Line> line_color) = 0;
        virtual void Line(const vec2& from, const vec2& to, const Line& line) = 0;
        virtual void Text(std::shared_ptr<Font> font, const std::string& text, const vec2& where, const Rgb& color) = 0;

        virtual TextSize GetSizeOfString(std::shared_ptr<Font> font, const std::string& str) = 0;
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

    std::shared_ptr<App> CreateApp(std::shared_ptr<Driver> driver, const Arguments& arguments);
}

#endif  // RIDE_DRIVER_H
