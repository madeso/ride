#include "driver.h"
#include <iostream>

namespace ride
{
    Rgb::Rgb(int rr, int gg, int bb)
        : r(rr)
        , g(gg)
        , b(bb)
    {
    }


    Line::Line(const Rgb& c, int w)
        : color(c)
        , width(w)
    {
    }


    vec2::vec2(int xx, int yy)
        : x(xx)
        , y(yy)
    {
    }


    Font::~Font()
    {
    }


    Driver::~Driver()
    {
    }


    Painter::~Painter()
    {
    }


    App::~App()
    {
    }


    struct RideApp : App
    {
        std::shared_ptr<Driver> driver;
        std::shared_ptr<Font> font_ui;
        std::shared_ptr<Font> font_code;

        vec2 window_size = vec2{0,0};
        std::optional<vec2> mouse = std::nullopt;
        std::optional<vec2> start = std::nullopt;

        float circle = 25.0f;

        bool on_left = false;

        std::string str = "void main() { return 42; }";

        RideApp(std::shared_ptr<Driver> d)
            : driver(d)
            , font_ui(d->CreateUiFont(14))
            , font_code(d->CreateCodeFont(14))
        {
        }

        void OnSize(const vec2& new_size) override
        {
            window_size = new_size;

            driver->Refresh();
        }

        void OnPaint(Painter* painter) override
        {
            painter->Rect(vec2(0, 0), window_size, Rgb{200, 200, 200}, std::nullopt);

            // draw some text
            painter->Text(font_ui, "File | Code | Help", {40, 20}, {0, 0, 0});
            painter->Text(font_code, str, {40,60}, {0,0,0});

            // draw a circle, green filling, 5-pixels-thick red outline
            if(mouse && !start)
            {
                painter->Circle(*mouse, static_cast<int>(circle), Rgb{0, 255, 0}, Line{{0,0,0}, 1});
            }
            
            // draw a rectangle, blue filling, 10-pixels-thick pink outline
            painter->Rect({on_left ? 0 : (window_size.x - 400), 100}, {400, 200}, Rgb{0, 0, 255}, Line{{0,0,0}, 1});
            
            // draw a black line, 3 pixels thick
            if(start && mouse)
            {
                painter->Line( *start, *mouse, {{0,0,0}, 3} ); // draw line across the rectangle
            }
        }

        void OnMouseMoved(const vec2& new_position) override
        {
            mouse = new_position;
            driver->Refresh();
        }

        void OnMouseLeftWindow() override
        {
            mouse = std::nullopt;
            driver->Refresh();
        }

        void OnMouseButton(MouseState state, MouseButton button) override
        {
            if(button == MouseButton::Left)
            {
                if(state == MouseState::Up)
                {
                    start = std::nullopt;
                }
                else
                {
                    if(mouse)
                    {
                        start = *mouse;
                    }
                }
                
            }
            driver->Refresh();
        }

        void OnMouseScroll(float scroll, int lines) override
        {
            circle = std::max(1.0f, circle + scroll * lines);
            driver->Refresh();
        }

        bool OnKey(bool down, Key key) override
        {
            if(down) { return false; }

            bool handled = false;
            
            switch(key)
            {
            case Key::Left: on_left = true; handled = true; break;
            case Key::Right: on_left = false; handled = true; break;
            case Key::Escape: str = ""; handled = true; break;
            default: break;
            }

            if(handled)
            {
                driver->Refresh();
            }

            return handled;
        }

        void OnChar(const std::string& ch) override
        {
            str += ch;
            driver->Refresh();
        }
    };

    std::shared_ptr<App>
    CreateApp(std::shared_ptr<Driver> driver)
    {
        return std::make_shared<RideApp>(driver);
    }
}
