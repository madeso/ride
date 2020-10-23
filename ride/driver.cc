#include "driver.h"

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
        vec2 window_size = vec2{0,0};
        std::optional<vec2> mouse = std::nullopt;
        std::optional<vec2> start = std::nullopt;

        RideApp(std::shared_ptr<Driver> d)
            : driver(d)
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
            painter->Text("Testing", {40,60}, {0,0,0});

            // draw a circle, green filling, 5-pixels-thick red outline
            if(mouse && !start)
            {
                painter->Circle(*mouse, 25, Rgb{0, 255, 0}, Line{{0,0,0}, 1});
            }
            
            // draw a rectangle, blue filling, 10-pixels-thick pink outline
            painter->Rect({window_size.x - 400, 100}, {400, 200}, Rgb{0, 0, 255}, Line{{0,0,0}, 1});
            
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
    };

    std::shared_ptr<App>
    CreateApp(std::shared_ptr<Driver> driver)
    {
        return std::make_shared<RideApp>(driver);
    }
}
