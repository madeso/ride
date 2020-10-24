#include "driver.h"

#include "ride/str.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace ride
{
    vec2 operator+(const vec2& lhs, const vec2& rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y};
    }


    Rect Rect::CreateWestFromMaxSize(int max_size) const
    {
        return {position, {std::min(max_size, size.x), size.y}};
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
    


    struct Document
    {
        std::vector<std::string> lines;

        Document()
        {
            LoadFile(__FILE__);
        }

        void LoadFile(const std::string& path)
        {
            std::cout << "Loading " << path << "\n";
            lines.clear();

            std::ifstream file;
            file.open(path.c_str(), std::ios::in | std::ios::binary);
            if(file.is_open() == false)
            {
                return;
            }

            std::string line;
            while(getline(file,line))
            {
                lines.emplace_back(line);
            }
        }
    };


    struct RectScope
    {
        Painter* painter;

        RectScope(Painter* p, const Rect& rect) : painter(p)
        {
            painter->PushClip(rect);
        }

        ~RectScope()
        {
            painter->PopClip();
        }
    };


    struct View
    {
        std::shared_ptr<Font> font;
        std::shared_ptr<Document> document;

        vec2 scroll = {0, 0};
        Rect rect = {{0, 0}, {0, 0}};

        void ScrollDown(int y)
        {
            scroll.y += y;
        }

        void ScrollRight(int x)
        {
            scroll.x += x;
        }

        void Draw
        (
            std::shared_ptr<Driver> driver,
            Painter* painter,
            const Rgb& foreground_color,
            const Rgb& background_color,
            const Rgb& gutter_color
        )
        {
            painter->Rect(rect, background_color, std::nullopt);

            if(font && document)
            {
                const auto scope = RectScope{painter, rect};

                const bool render_linenumber = true;
                const int left_gutter_padding = 3;
                const int right_gutter_padding = 6;
                const int editor_padding_left = 6;

                const auto line_number_size = !render_linenumber ? 0 : driver->GetSizeOfString(font, Str{} << document->lines.size() + 1).width;

                const auto gutter_rect = rect.CreateWestFromMaxSize(left_gutter_padding + line_number_size + right_gutter_padding);
                painter->Rect(gutter_rect, gutter_color, std::nullopt);
                
                const auto lower_right = rect.position + rect.size;
                const auto line_height = driver->GetSizeOfString(font, "ABCgdijlk").height;
                
                auto draw = rect.position;
                auto current_scroll = scroll;

                for(; draw.y < lower_right.y; draw.y += line_height)
                {
                    if(current_scroll.y >= 0 && current_scroll.y < document->lines.size())
                    {
                        const auto substr = [](const std::string& str, int offset) -> std::string
                        {
                            if(offset >= static_cast<int>(str.length())) { return ""; }
                            else if(offset < 0)
                            {
                                return std::string(-offset, ' ') + str;
                            }
                            else
                            {
                                return str.substr(offset);
                            }
                        };
                        if(render_linenumber && current_scroll.y >= 0)
                        {
                            painter->Text(font, Str{} << current_scroll.y + 1, {draw.x + left_gutter_padding, draw.y}, foreground_color);
                        }
                        const auto line = substr(document->lines[current_scroll.y], scroll.x);

                        painter->Text(font, line, {draw.x + gutter_rect.size.x + editor_padding_left, draw.y}, foreground_color);
                    }

                    current_scroll.y = current_scroll.y + 1;
                }
            }
        }
    };


    const std::string MEASSURE_STRING = "ABCdefjklm";


    struct RideApp : App
    {
        std::shared_ptr<Driver> driver;
        std::shared_ptr<Font> font_ui;
        std::shared_ptr<Font> font_code;
        std::shared_ptr<Font> font_big;
        TextSize text_size;

        View view;

        vec2 window_size = vec2{0,0};
        std::optional<vec2> mouse = std::nullopt;
        std::optional<vec2> start = std::nullopt;

        float circle = 25.0f;

        bool on_left = false;

        std::string str = "void main() { return 42; }";

        RideApp(std::shared_ptr<Driver> d)
            : driver(d)
            , font_ui(d->CreateUiFont(12))
            , font_code(d->CreateCodeFont(8))
            , font_big(d->CreateUiFont(100))
            , text_size(d->GetSizeOfString(font_big, MEASSURE_STRING))
        {
            view.font = font_code;
            view.rect = Rect{{10, 20}, {400, 420}};
            view.document = std::make_shared<Document>();
            std::cout << text_size.external_leading << "\n";
        }

        void OnSize(const vec2& new_size) override
        {
            window_size = new_size;

            driver->Refresh();
        }

        void OnPaint(Painter* painter) override
        {
            painter->Rect({{0, 0}, window_size}, Rgb{230, 230, 230}, std::nullopt);

            painter->Text(font_ui, "File | Code | Help", {40, 00}, {0, 0, 0});

            // draw a circle, green filling, 5-pixels-thick red outline
            if(mouse && !start)
            {
                painter->Circle(*mouse, static_cast<int>(circle), Rgb{0, 255, 0}, Line{{0,0,0}, 1});
            }
            
            // draw a black line, 3 pixels thick
            if(start && mouse)
            {
                painter->Line( *start, *mouse, {{0,0,0}, 3} ); // draw line across the rectangle
            }

            view.Draw(driver, painter, {0,0,0}, {180, 180, 180}, {160, 160, 160});
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

        bool ctrl = false;
        bool OnKey(bool down, Key key) override
        {
            if(key == Key::Control)
            {
                ctrl = down;
                driver->Refresh();
                return true;
            }

            bool handled = false;

            switch(key)
            {
            case Key::Escape: str = ""; handled = true; break;
            case Key::Left:
                if(ctrl && down)
                {
                    view.ScrollRight(-1);
                }
                handled = true;
                break;
            case Key::Right:
                if(ctrl && down)
                {
                    view.ScrollRight(1);
                }
                handled = true;
                break;
            case Key::Up:
                if(ctrl && down)
                {
                    view.ScrollDown(-1);
                }
                handled = true;
                break;
            case Key::Down:
                if(ctrl && down)
                {
                    view.ScrollDown(1);
                }
                handled = true;
                break;

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
