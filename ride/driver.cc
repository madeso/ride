#include "driver.h"

#include "ride/str.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

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
        std::vector<std::string> lines =
        {
            "abcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHI",
            "         111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999",
            "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789",
            ""
        };
        
        int GetNumberOfLines() const
        {
            return lines.size();
        }

        std::string GetLineAt(int y) const
        {
            if(y < 0) { return ""; }
            else if(y >= lines.size()) { return ""; }
            else { return lines[y]; }
        }

        Document()
        {
            LoadFile(__FILE__);
        }

        void LoadFile(const std::string& path)
        {
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


    struct Settings
    {
        vec2 scroll_spacing = {3, 3};

        bool render_linenumber = true;
        int left_gutter_padding = 3;
        int right_gutter_padding = 6;
        int editor_padding_left = 6;
    };


    struct View
    {
        std::shared_ptr<Driver> driver;
        std::shared_ptr<Font> font;
        std::shared_ptr<Document> document;
        std::shared_ptr<Settings> settings;

        int requested_cursor_x = 0;
        vec2 cursor = {0, 0};
        vec2 scroll = {0, 0};
        Rect rect = {{0, 0}, {0, 0}};

        void StepRight(int x)
        {
            const int next_x = std::max(0, cursor.x + x);
            if(document && cursor.y < document->GetNumberOfLines())
            {
                cursor.x = std::min<int>(document->GetLineAt(cursor.y).length(), next_x);
            }
            requested_cursor_x = cursor.x;

            FocusCursor();
        }

        int GetLineNumberSize()
        {
            if(settings == nullptr) { return 0; }
            if(document == nullptr) { return 0; }
            if(driver == nullptr) { return 0; }

            if(settings->render_linenumber == false) { return 0; }
            
            return driver->GetSizeOfString(font, Str{} << document->GetNumberOfLines() + 1).width;
        }

        void StepDown(int y)
        {
            if(document)
            {
                cursor.y = std::min<int>(std::max(0, cursor.y + y), document->GetNumberOfLines());
                cursor.x = std::min<int>(document->GetLineAt(cursor.y).length(), requested_cursor_x);
            }

            FocusCursor();
        }

        void LimitScroll()
        {
            if(document == nullptr) { return; }
            if(settings == nullptr) { return; }

            const auto limy = [this](int lines) { if(scroll.y > lines) { scroll.y = lines; } };

            // limy(document->GetNumberOfLines() + 1);
            limy(document->GetNumberOfLines() + 1 - GetWindowHeightInLines());

            // todo(Gustav): also limit based on width

            if(scroll.x < 0) { scroll.x = 0; }
            if(scroll.y < 0) { scroll.y = 0; }
        }

        void ScrollDown(int y)
        {
            scroll.y += y;
            LimitScroll();
        }

        void ScrollRight(int x)
        {
            scroll.x += x;
            LimitScroll();
        }

        int GetWindowHeightInLines() const
        {
            if(font == nullptr) { return 0; }
            if(font->line_height <= 0) { return 0; }

            const auto lines = rect.size.y / static_cast<float>(font->line_height);
            return static_cast<int>(std::floor(lines));
        }

        int GetWindowWidthInChars()
        {
            if(font == nullptr) { return 0; }
            if(font->char_width <= 0) { return 0; }

            const auto gutter_width = settings->left_gutter_padding + settings->right_gutter_padding + GetLineNumberSize() + settings->editor_padding_left;
            const auto window_width = std::max(0, rect.size.x - gutter_width);

            const auto chars = window_width / static_cast<float>(font->char_width);
            const auto ci = static_cast<int>(std::floor(chars));

            return ci;
        }

        void FocusCursorHeight()
        {
            if(settings == nullptr) { return; }

            const auto window_height = GetWindowHeightInLines();

            const auto cursor_up = cursor.y - settings->scroll_spacing.y;
            const auto cursor_down = cursor.y + settings->scroll_spacing.y;

            if(scroll.y > cursor_up)
            {
                scroll.y = cursor_up;
            }

            {
                const auto steps = scroll.y + window_height - cursor_down -1;
                if(steps < 0)
                {
                    scroll.y -= steps;
                }
            }
        }
        void FocusCursorWidth()
        {
            if(settings == nullptr) { return; }

            const auto window_width = GetWindowWidthInChars();

            const auto cursor_left = cursor.x - settings->scroll_spacing.x;
            const auto cursor_right = cursor.x + settings->scroll_spacing.x;

            if(scroll.x > cursor_left)
            {
                scroll.x = cursor_left;
            }

            {
                const auto steps = scroll.x + window_width - cursor_right;
                if(steps < 0)
                {
                    scroll.x -= steps;
                }
            }
        }

        void FocusCursor()
        {
            FocusCursorHeight();
            FocusCursorWidth();
            LimitScroll();
        }

        void Draw
        (
            Painter* painter,
            const Rgb& foreground_color,
            const Rgb& background_color,
            const Rgb& gutter_color
        )
        {
            const auto linenumber_color = foreground_color;

            if(settings == nullptr) { return; }
            if(driver == nullptr) { return; }

            painter->Rect(rect, background_color, std::nullopt);

            if(font && document)
            {
                const auto scope = RectScope{painter, rect};

                const bool render_linenumber = settings->render_linenumber;
                const int left_gutter_padding = settings->left_gutter_padding;
                const int right_gutter_padding = settings->right_gutter_padding;
                const int editor_padding_left = settings->editor_padding_left;

                const auto line_number_size = GetLineNumberSize();

                const auto gutter_rect = rect.CreateWestFromMaxSize(left_gutter_padding + line_number_size + right_gutter_padding);
                painter->Rect(gutter_rect, gutter_color, std::nullopt);
                
                const auto lower_right = rect.position + rect.size;
                
                auto draw = rect.position;
                auto current_scroll = scroll;

                for(; draw.y < lower_right.y; draw.y += font->line_height)
                {
                    if(current_scroll.y >= 0 && current_scroll.y < document->GetNumberOfLines()+1)
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
                            painter->Text(font, Str{} << current_scroll.y + 1, {draw.x + left_gutter_padding, draw.y}, linenumber_color);
                        }
                        const auto full_line = document->GetLineAt(current_scroll.y);
                        const auto line = substr(full_line, scroll.x);
                        const auto draw_position = vec2{draw.x + gutter_rect.size.x + editor_padding_left, draw.y};

                        if(current_scroll.y == cursor.y)
                        {
                            const auto cursor_x = driver->GetSizeOfString(font, full_line.substr(0, cursor.x - scroll.x)).width;
                            painter->Line
                            (
                                {draw_position.x + cursor_x, draw_position.y},
                                {draw_position.x + cursor_x, draw_position.y + font->line_height},
                                {{0, 0, 0}, 1}
                            );
                        }

                        painter->Text(font, line, draw_position, foreground_color);
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
            view.settings = std::make_shared<Settings>();
            view.driver = driver;
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

            view.Draw(painter, {0,0,0}, {180, 180, 180}, {160, 160, 160});
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
                if(down)
                {
                    if(ctrl) { view.ScrollRight(-1); }
                    else { view.StepRight(-1); }
                }
                handled = true;
                break;
            case Key::Right:
                if(down)
                {
                    if(ctrl) { view.ScrollRight(1); }
                    else { view.StepRight(1); }
                }
                handled = true;
                break;
            case Key::Up:
                if(down)
                {
                    if(ctrl) { view.ScrollDown(-1); }
                    else { view.StepDown(-1); }
                }
                handled = true;
                break;
            case Key::Down:
                if(down)
                {
                    if(ctrl) { view.ScrollDown(1); }
                    else { view.StepDown(1); }
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
