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

    struct Painter
    {
        virtual ~Painter();
        
        virtual void Rect(const vec2& point, const vec2& size, std::optional<Rgb> fill, std::optional<Line> line_color) = 0;
        virtual void Circle(const vec2& point, int radius, std::optional<Rgb> fill, std::optional<Line> line_color) = 0;

        virtual void Line(const vec2& from, const vec2& to, const Line& line) = 0;
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
    };

    std::shared_ptr<App> CreateApp(std::shared_ptr<Driver> driver);
}

#endif  // RIDE_DRIVER_H
