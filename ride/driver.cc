#include "driver.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <functional>
#include <algorithm>
#include <cassert>

#include <filesystem>
#include <system_error>

#include "ride/str.h"
#include "ride/humane_sort.h"


namespace ride
{
    template<typename T>
    T KeepWithin(T min, T t, T max)
    {
        assert(min <= max);

        if(t < min) { return min; }
        else if(t > max) { return max; }
        else { return t; }
    }


    std::string ToString(const Key key)
    {
        switch(key)
        {
            case Key::None: return "None";
            case Key::Backspace: return "Backspace";
            case Key::Tab: return "Tab";
            case Key::Return: return "Return";
            case Key::Escape: return "Escape";
            case Key::Space: return "Space";
            case Key::Delete: return "Delete";
            case Key::Start: return "Start";
            case Key::LButton: return "LButton";
            case Key::RButton: return "RButton";
            case Key::Cancel: return "Cancel";
            case Key::MButton: return "MButton";
            case Key::Clear: return "Clear";
            case Key::Shift: return "Shift";
            case Key::Alt: return "Alt";
            case Key::Control: return "Control";
            case Key::Menu: return "Menu";
            case Key::Pause: return "Pause";
            case Key::Capital: return "Capital";
            case Key::End: return "End";
            case Key::Home: return "Home";
            case Key::Left: return "Left";
            case Key::Up: return "Up";
            case Key::Right: return "Right";
            case Key::Down: return "Down";
            case Key::Select: return "Select";
            case Key::Print: return "Print";
            case Key::Execute: return "Execute";
            case Key::Snapshot: return "Snapshot";
            case Key::Insert: return "Insert";
            case Key::Help: return "Help";
            case Key::Numpad0: return "Numpad0";
            case Key::Numpad1: return "Numpad1";
            case Key::Numpad2: return "Numpad2";
            case Key::Numpad3: return "Numpad3";
            case Key::Numpad4: return "Numpad4";
            case Key::Numpad5: return "Numpad5";
            case Key::Numpad6: return "Numpad6";
            case Key::Numpad7: return "Numpad7";
            case Key::Numpad8: return "Numpad8";
            case Key::Numpad9: return "Numpad9";
            case Key::Multiply: return "Multiply";
            case Key::Add: return "Add";
            case Key::Separator: return "Separator";
            case Key::Subtract: return "Subtract";
            case Key::Decimal: return "Decimal";
            case Key::Divide: return "Divide";
            case Key::F1: return "F1";
            case Key::F2: return "F2";
            case Key::F3: return "F3";
            case Key::F4: return "F4";
            case Key::F5: return "F5";
            case Key::F6: return "F6";
            case Key::F7: return "F7";
            case Key::F8: return "F8";
            case Key::F9: return "F9";
            case Key::F10: return "F10";
            case Key::F11: return "F11";
            case Key::F12: return "F12";
            case Key::F13: return "F13";
            case Key::F14: return "F14";
            case Key::F15: return "F15";
            case Key::F16: return "F16";
            case Key::F17: return "F17";
            case Key::F18: return "F18";
            case Key::F19: return "F19";
            case Key::F20: return "F20";
            case Key::F21: return "F21";
            case Key::F22: return "F22";
            case Key::F23: return "F23";
            case Key::F24: return "F24";
            case Key::Numlock: return "Numlock";
            case Key::Scroll: return "Scroll";
            case Key::PageUp: return "PageUp";
            case Key::PageDown: return "PageDown";
            case Key::NumpadSpace: return "NumpadSpace";
            case Key::NumpadEnter: return "NumpadEnter";
            case Key::NumpadF1: return "NumpadF1";
            case Key::NumpadF2: return "NumpadF2";
            case Key::NumpadF3: return "NumpadF3";
            case Key::NumpadF4: return "NumpadF4";
            case Key::NumpadHome: return "NumpadHome";
            case Key::NumpadLeft: return "NumpadLeft";
            case Key::NumpadUp: return "NumpadUp";
            case Key::NumpadRight: return "NumpadRight";
            case Key::NumpadDown: return "NumpadDown";
            case Key::NumpadPageUp: return "NumpadPageUp";
            case Key::NumpadPageDown: return "NumpadPageDown";
            case Key::NumpadEnd: return "NumpadEnd";
            case Key::NumpadBegin: return "NumpadBegin";
            case Key::NumpadInsert: return "NumpadInsert";
            case Key::NumpadDelete: return "NumpadDelete";
            case Key::NupadEqual: return "NupadEqual";
            case Key::NumpadMultiply: return "NumpadMultiply";
            case Key::NumpadAdd: return "NumpadAdd";
            case Key::NumpadSeparator: return "NumpadSeparator";
            case Key::NumpadSubtract: return "NumpadSubtract";
            case Key::NumpadDecimal: return "NumpadDecimal";
            case Key::NumpadDivide: return "NumpadDivide";
            case Key::WindowLeft: return "WindowLeft";
            case Key::WindowsRight: return "WindowsRight";
            case Key::WindowsMenu: return "WindowsMenu";
            case Key::Special1: return "Special1";
            case Key::Special2: return "Special2";
            case Key::Special3: return "Special3";
            case Key::Special4: return "Special4";
            case Key::Special5: return "Special5";
            case Key::Special6: return "Special6";
            case Key::Special7: return "Special7";
            case Key::Special8: return "Special8";
            case Key::Special9: return "Special9";
            case Key::Special10: return "Special10";
            case Key::Special11: return "Special11";
            case Key::Special12: return "Special12";
            case Key::Special13: return "Special13";
            case Key::Special14: return "Special14";
            case Key::Special15: return "Special15";
            case Key::Special16: return "Special16";
            case Key::Special17: return "Special17";
            case Key::Special18: return "Special18";
            case Key::Special19: return "Special19";
            case Key::Special20: return "Special20";

            case Key::ExclamationMark: return "!";
            case Key::DoubleQuotes: return "\"";
            case Key::Number: return "#";
            case Key::Dollar: return "$";
            case Key::Percent: return "%";
            case Key::Ampersand: return "&";
            case Key::SingleQuote: return "'";
            case Key::OpenParenthesis: return "(";
            case Key::CloseParenthesis: return ")";
            case Key::Asterisk: return "*";
            case Key::Plus: return "+";
            case Key::Comma: return ",";
            case Key::Hyphen: return "-";
            case Key::Dot: return ".";
            case Key::Slash: return "/";
            case Key::Zero: return "0";
            case Key::One: return "1";
            case Key::Two: return "2";
            case Key::Three: return "3";
            case Key::Four: return "4";
            case Key::Five: return "5";
            case Key::Six: return "6";
            case Key::Seven: return "7";
            case Key::Eight: return "8";
            case Key::Nine: return "9";
            case Key::Colon: return ":";
            case Key::Semicolon: return ";";
            case Key::LessThan: return "<";
            case Key::Equals: return "=";
            case Key::GreaterThan: return ">";
            case Key::QuestionMark: return "?";
            case Key::At: return "@";
            case Key::UppercaseA: return "A";
            case Key::UppercaseB: return "B";
            case Key::UppercaseC: return "C";
            case Key::UppercaseD: return "D";
            case Key::UppercaseE: return "E";
            case Key::UppercaseF: return "F";
            case Key::UppercaseG: return "G";
            case Key::UppercaseH: return "H";
            case Key::UppercaseI: return "I";
            case Key::UppercaseJ: return "J";
            case Key::UppercaseK: return "K";
            case Key::UppercaseL: return "L";
            case Key::UppercaseM: return "M";
            case Key::UppercaseN: return "N";
            case Key::UppercaseO: return "O";
            case Key::UppercaseP: return "P";
            case Key::UppercaseQ: return "Q";
            case Key::UppercaseR: return "R";
            case Key::UppercaseS: return "S";
            case Key::UppercaseT: return "T";
            case Key::UppercaseU: return "U";
            case Key::UppercaseV: return "V";
            case Key::UppercaseW: return "W";
            case Key::UppercaseX: return "X";
            case Key::UppercaseY: return "Y";
            case Key::UppercaseZ: return "Z";
            case Key::OpeningBracket: return "[";
            case Key::Backslash: return "\\";
            case Key::ClosingBracket: return "]";
            case Key::Caret: return "^";
            case Key::Underscore: return "_";
            case Key::GraveAccent: return "`";
            case Key::LowercaseA: return "a";
            case Key::LowercaseB: return "b";
            case Key::LowercaseC: return "c";
            case Key::LowercaseD: return "d";
            case Key::LowercaseE: return "e";
            case Key::LowercaseF: return "f";
            case Key::LowercaseG: return "g";
            case Key::LowercaseH: return "h";
            case Key::LowercaseI: return "i";
            case Key::LowercaseJ: return "j";
            case Key::LowercaseK: return "k";
            case Key::LowercaseL: return "l";
            case Key::LowercaseM: return "m";
            case Key::LowercaseN: return "n";
            case Key::LowercaseO: return "o";
            case Key::LowercaseP: return "p";
            case Key::LowercaseQ: return "q";
            case Key::LowercaseR: return "r";
            case Key::LowercaseS: return "s";
            case Key::LowercaseT: return "t";
            case Key::LowercaseU: return "u";
            case Key::LowercaseV: return "v";
            case Key::LowercaseW: return "w";
            case Key::LowercaseX: return "x";
            case Key::LowercaseY: return "y";
            case Key::LowercaseZ: return "z";
            case Key::OpeningBrace: return "{";
            case Key::VerticalVar: return "|";
            case Key::ClosingBrace: return "}";
            case Key::Tilde: return "~";
        }
    }

    struct Theme
    {
        // todo(Gustav): refactor out to a palette with named colors
        Rgb background = {230, 230, 230};
        
        Rgb scrollbar_background_color = {120, 120, 120};
        Rgb scrollbar_scrollbar_color = {210, 210, 210};
        Rgb scrollbar_line_color = {50, 50, 50};
        Rgb scrollbar_button_color = {150, 150, 150};

        Rgb text_foreground_color = {0,0,0};
        Rgb text_background_color = {180, 180, 180};
        Rgb text_gutter_color = {160, 160, 160};
        Rgb text_linenumber_color = {0,0,0};
        Rgb text_cursor = {0,0,0};

        Rgb statusbar_text_color = {0, 0, 0};
        Rgb statusbar_bkg = {200, 200, 200};

        Rgb tabbar_background_color = {200, 200, 200};
        Rgb tabbar_tab_inactive_background = {180, 180, 180};
        Rgb tabbar_tab_selected_background = {220, 220, 220};
        Rgb tabbar_tab_line_color = {0, 0, 0};
        Rgb tabbar_tab_text_color = {0, 0, 0};

        Rgb filesys_background_color = {180, 180, 180};
        Rgb filesys_folder_color = {0, 0, 0};
        Rgb filesys_file_color = {40, 40, 40};
        Rgb filesys_hidden_color = {80, 80, 80};

        Rgb edit_background = {110, 110, 110};
        Rgb edit_selection_bkg = {80, 80, 80};
        Rgb edit_cursor = {0, 0, 0};
        Rgb edit_text = {0, 0, 0};

        Rgb command_view_background = {80, 80, 80};
    };

    struct Settings
    {
        Theme theme;

        int min_size_of_scrollbar = 3;

        int window_padding = 3;

        // number of extra lines to display in view
        vec2 scroll_spacing = {3, 3};

        bool render_linenumber = true;
        int left_gutter_padding = 3;
        int right_gutter_padding = 6;

        int editor_padding_left = 6;

        int statusbar_padding_bottom = 3;
        int statusbar_padding_top = 3;
        int statusbar_padding_right = 6;

        bool scroll_to_cursor_on_click = true;
        int lines_to_scroll_for_scrollbar_button = 3;

        bool directories_first = true;
        bool sort_files = true;

        int tab_padding_left = 6;
        int tab_padding_right = 6;
        int tab_height_padding = 3;
        int tab_spacing = 3;
        int tab_start_padding = 3;
        int tab_end_padding = 3;
        int selected_tab_height = 6;

        // how many pixels is a line when scrolling in the tab bar
        int tab_scroll_speed = 40;

        // the width of the scrollbar
        int scrollbar_width = 10;

        int commandview_height = 300;
        int commandview_width = 400;
        int commandview_edit_inset = 6;
        int commandview_edit_extra_height = 3;
    };


    struct Callbacks
    {
        using Callback = std::function<void ()>;

        std::vector<Callback> callbacks;

        void Add(Callback&& c)
        {
            callbacks.emplace_back(std::move(c));
        }

        void Call()
        {
            for(const auto& c: callbacks)
            {
                c();
            }
        }
    };


    struct FileEntry
    {
        std::string name;
        std::string path;
        bool is_directory;

        FileEntry(const std::string& n, const std::string& p, bool d) : name(n), path(p), is_directory(d) {}
    };


    struct FileSystem
    {
        std::optional<std::string> GetCurrentDirectory() const
        {
            std::error_code error;
            const auto p = std::filesystem::current_path(error);
            if(error)
            {
                std::cerr << "Error getting current directory: " << error << "\n";
                return std::nullopt;
            }
            else
            {
                return p.u8string();
            }
        }

        std::optional<std::string> AsAbsolute(const std::string& p)
        {
            std::error_code error;

            const auto relative = std::filesystem::path(p);
            const auto absolute = std::filesystem::canonical(relative, error);
            if(error)
            {
                std::cerr << "Error making path '" << p << "'absolute: " << error << "\n";
                return std::nullopt;
            }

            return absolute.u8string();
        }

        std::optional<bool> Exists(const std::string& p)
        {
            const auto path = std::filesystem::path(p);
            std::error_code error;
            const bool exists = std::filesystem::exists(path, error);
            if(error)
            {
                std::cerr << "Error checking '" << p << "' if it exists: " << error << "\n";
                return std::nullopt;
            }

            return exists;
        }

        std::optional<std::vector<FileEntry>> List(const std::string& dir, const Settings& settings) const
        {
            const auto should_sort = settings.sort_files;
            const auto sort = [should_sort](std::vector<FileEntry>* files)
            {
                if(should_sort == false) { return; }
                std::sort(files->begin(), files->end(), [](const FileEntry& lhs, const FileEntry& rhs) {
                    return humane_strcmp(lhs.name.c_str(), rhs.name.c_str()) < 0;
                });
            };

            std::vector<FileEntry> files;
            std::vector<FileEntry> folders;

            std::error_code error;
            auto directory_iterator = std::filesystem::directory_iterator(dir, error);
            if(error)
            {
                std::cerr << "Error listing file in " << dir << ": " << error << "\n";
                return std::nullopt;
            }

            for(auto& iterator: directory_iterator)
            {
                const auto p = iterator.path();
                const auto file_name = p.filename().u8string();
                const auto file_path = p.u8string();
                if(iterator.is_directory())
                {
                    folders.emplace_back(file_name, file_path, true);
                }
                else if(iterator.is_regular_file())
                {
                    files.emplace_back(file_name, file_path, false);
                }
                // ignore other types
            }

            if(settings.directories_first)
            {
                sort(&files);
                sort(&folders);
                folders.insert(folders.end(), files.begin(), files.end());
                return folders;
            }
            else
            {
                folders.insert(folders.end(), files.begin(), files.end());
                sort(&folders);
                return folders;
            }
        }
    };

    std::shared_ptr<FileSystem> MakeFs()
    {
        return std::make_shared<FileSystem>();
    }

    int C(std::size_t i)  { return static_cast<int>(i); }
    std::size_t Cs(int i)  { return static_cast<size_t>(i); }

    vec2 operator+(const vec2& lhs, const vec2& rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y};
    }

    vec2 operator-(const vec2& lhs, const vec2& rhs)
    {
        return {lhs.x - rhs.x, lhs.y - rhs.y};
    }


    bool Rect::Contains(const vec2& p) const
    {
        const auto cx = p.x >= position.x && position.x + size.x >= p.x;
        const auto cy = p.y >= position.y && position.y + size.y >= p.y;
        return cx && cy;
    }


    Rect Rect::CreateWestFromMaxSize(int max_size) const
    {
        return {position, {std::min(max_size, size.x), size.y}};
    }


    Rect Rect::CreateNorthFromMaxSize(int max_size) const
    {
        return {position, {size.x, std::min(max_size, size.y)}};
    }

    Rect Rect::CreateSouthFromMaxSize(int max_size) const
    {
        const auto s = std::min(max_size, size.y);
        return {{position.x, position.y + size.y - s}, {size.x, s}};
    }

    Rect Rect::CreateEastFromMaxSize(int max_size) const
    {
        const auto s = std::min(max_size, size.x);
        return {{position.x + size.x - s, position.y}, {s, size.y}};
    }

    Rect Rect::Offset(const vec2& offset) const
    {
        return {position + offset, size};
    }

    Rect Rect::CreateFromCenterMaxSize(int max_size) const
    {
        const auto s = std::min(max_size, size.x);
        const auto h = static_cast<int>(static_cast<float>(size.x - s) / 2.0f);
        return {{position.x + h, position.y}, {s, size.y}};
    }

    Rect Rect::Inset(int inset) const
    {
        // todo(Gustav): handle when inset is greater than size
        return {{position.x+inset, position.y+inset}, {size.x - inset*2, size.y - inset*2}};
    }

    int Rect::GetTop() const
    {
        return position.y;
    }

    int Rect::GetBottom() const
    {
        return position.y + size.y;
    }

    int Rect::GetLeft() const
    {
        return position.x;
    }

    int Rect::GetRight() const
    {
        return position.x + size.x;
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
            return C(lines.size());
        }

        std::string GetLineAt(int y) const
        {
            if(y < 0) { return ""; }
            else if(y >= C(lines.size())) { return ""; }
            else { return lines[Cs(y)]; }
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


    struct DocumentInformation
    {
        int line;
        int column;
        int number_of_lines;

        DocumentInformation(int l, int c, int n) : line(l), column(c), number_of_lines(n) {}
    };


    using DocumentInformationProvider = std::function<DocumentInformation ()>;


    struct StatusBar
    {
        std::shared_ptr<Font> font;
        std::shared_ptr<Settings> settings;
        DocumentInformationProvider information_provider;

        StatusBar
        (
            std::shared_ptr<Font> f,
            std::shared_ptr<Settings> s,
            DocumentInformationProvider ip
        ) : font(f), settings(s), information_provider(ip) {}

        void Draw(Painter* painter, const vec2& window_size)
        {
            const auto text_color = settings->theme.statusbar_text_color;
            const auto bkg = settings->theme.statusbar_bkg;

            const auto info = information_provider();

            const auto draw_height = GetHeight();
            const auto rect = Rect{{0, window_size.y - draw_height}, {window_size.x, draw_height}};

            painter->Rect(rect, bkg, std::nullopt);

            const std::string str = Str{}
                << "Ln " << info.line << " / " << info.number_of_lines << " | "
                << "Col " << info.column
                ;

            const auto text_size = painter->GetSizeOfString(font, str);
            const auto text_pos = vec2
            {
                rect.position.x + rect.size.x - text_size.width - settings->statusbar_padding_right,
                rect.position.y + settings->statusbar_padding_top
            };

            painter->Text(font, str, text_pos, text_color);
        }

        int GetHeight() const
        {
            if(font == nullptr) { return 0; }
            if(settings == nullptr) { return 0; }

            return font->line_height + settings->statusbar_padding_bottom + settings->statusbar_padding_top;
        }
    };


    struct Meta
    {
        bool ctrl;
        bool shift;
        bool alt;

        Meta(bool c, bool s, bool a) : ctrl(c), shift(s), alt(a) {}
    };


    struct View
    {
        virtual ~View() = default;

        virtual Rect GetRect() const = 0;

        virtual void Draw(Painter* painter) = 0;
        virtual void OnKey(Key key, const Meta& meta) = 0;
        virtual void OnChar(const std::string& ch) = 0;
        virtual void OnScroll(float scroll, int lines) = 0;

        virtual void MouseClick(const MouseButton& button, const MouseState state, const vec2& pos) = 0;
        virtual void MouseMoved(const vec2& pos) = 0;

        Callbacks on_change;
        void ViewChanged()
        {
            on_change.Call();
        }
    };


    struct ScrollableView : public View
    {
        std::shared_ptr<Settings> settings;

        Rect window_rect = EmptyRect;
        vec2 pixel_scroll = {0, 0};

        ScrollableView(std::shared_ptr<Settings> s) : settings(s) {}

        // get the full document area
        virtual vec2 GetDocumentSize() = 0;

        // todo(Gustav): store gutter width calculation as a optional std::function
        // and use that instead of virtual functions... this allows the children
        // to use meaningful names instead of the "west" and "main" lingo

        // how wide should the gutter area be?
        virtual int GetWestWidth() = 0;

        // vec2 GlobalToLocal(const vec2& global) const;
        // vec2 LocalToGlobal(const vec2& local) const;

        void LimitScroll()
        {
            const auto document_size = GetDocumentSize();

            // if document size < 0 then if is considerd 'infinite' so there is no limit

            if(document_size.y > 0)
            {
                if(pixel_scroll.y > (document_size.y - window_rect.size.y))
                {
                    pixel_scroll.y = document_size.y - window_rect.size.y;
                }
            }

            if(document_size.x > 0)
            {
                if(pixel_scroll.x > (document_size.x - window_rect.size.x))
                {
                    pixel_scroll.x = document_size.x - window_rect.size.x;
                }
            }

            if(pixel_scroll.x < 0) { pixel_scroll.x = 0; }
            if(pixel_scroll.y < 0) { pixel_scroll.y = 0; }
        }

        void ScrollDownPixels(int y)
        {
            pixel_scroll.y += y;
            LimitScroll();
            ViewChanged();
        }

        void ScrollRightPixels(int x)
        {
            pixel_scroll.x += x;
            LimitScroll();
            ViewChanged();
        }

        void ScrollToRectHeight(const Rect& rect)
        {
            const auto cursor_up = rect.GetTop();
            const auto cursor_down = rect.GetBottom();

            if(pixel_scroll.y > cursor_up)
            {
                pixel_scroll.y = cursor_up;
            }

            {
                const auto steps = pixel_scroll.y + window_rect.size.y - cursor_down -1;
                if(steps < 0)
                {
                    pixel_scroll.y -= steps;
                }
            }
        }

        void ScrollToRectWidth(const Rect& rect)
        {
            const auto cursor_left = rect.GetLeft();
            const auto cursor_right = rect.GetRight();

            if(pixel_scroll.x > cursor_left)
            {
                pixel_scroll.x = cursor_left;
            }

            {
                const auto scrollbar_width = ShowScrollbarVertical() ? window_rect.CreateEastFromMaxSize(settings->scrollbar_width).size.x : 0;
                const auto steps = pixel_scroll.x + (window_rect.size.x - scrollbar_width - GetWestWidth()) - cursor_right;
                if(steps < 0)
                {
                    pixel_scroll.x -= steps;
                }
            }
        }

        void ScrollToRect(const Rect& rect)
        {
            ScrollToRectHeight(rect);
            ScrollToRectWidth(rect);
            LimitScroll();
        }

        struct ScrollbarData
        {
            int size_without_buttons;
            Rect rect;
            Rect up_button;
            Rect down_button;
            Rect scrollbar;
            int area_to_scroll;
            int max_scroll;

            ScrollbarData
            (
                int s,
                const Rect& re,
                const Rect& up,
                const Rect& dw,
                const Rect& sc,
                int as,
                int ms
            )
                : size_without_buttons(s)
                , rect(re)
                , up_button(up)
                , down_button(dw)
                , scrollbar(sc)
                , area_to_scroll(as)
                , max_scroll(ms)
            {
            }
        };

        ScrollbarData GetVerticalScrollbarData()
        {
            const int scroll = pixel_scroll.y;
            const int document_size = GetDocumentSize().y;
            const Rect rect = window_rect.CreateEastFromMaxSize(settings->scrollbar_width);

            const auto button_height = rect.size.x;

            const auto up_button = rect.CreateNorthFromMaxSize(button_height);
            const auto down_button = rect.CreateSouthFromMaxSize(button_height);

            const auto size_without_buttons = rect.size.y - (up_button.size.y + down_button.size.y);

            const auto scrollbar_ratio = KeepWithin(0.0f, static_cast<float>(rect.size.y)/static_cast<float>(document_size), 1.0f);
            const auto suggest_scrollbar_size = scrollbar_ratio * static_cast<float>(size_without_buttons);
            const auto scrollbar_size = std::max(settings->min_size_of_scrollbar, static_cast<int>(std::ceil(suggest_scrollbar_size)));

            const auto area_to_scroll = std::max(0, size_without_buttons - scrollbar_size);

            const auto max_scroll = document_size - rect.size.y;

            const auto suggest_scroll_position = (static_cast<float>(scroll) / static_cast<float>(max_scroll)) * static_cast<float>(area_to_scroll);
            const auto scroll_position = static_cast<int>(std::floor(suggest_scroll_position));

            const auto scrollbar = Rect{{rect.position.x, rect.position.y + scroll_position + up_button.size.y}, {rect.size.x, scrollbar_size}};

            return
            {
                size_without_buttons,
                rect,
                up_button,
                down_button,
                scrollbar,
                area_to_scroll,
                max_scroll
            };
        }

        void DrawScrollbarVertical(Painter* painter)
        {
            const auto data = GetVerticalScrollbarData();

            if(data.size_without_buttons <= 0) { return; }

            const auto background_color = settings->theme.scrollbar_background_color;
            const auto scrollbar_color = settings->theme.scrollbar_scrollbar_color;
            const auto line_color = settings->theme.scrollbar_line_color;
            const auto button_color = settings->theme.scrollbar_button_color;

            const auto line = Line{line_color, 1};

            painter->Rect(data.rect, background_color, line);
            painter->Rect(data.up_button, button_color, line);
            painter->Rect(data.down_button, button_color, line);

            painter->Rect(data.scrollbar, scrollbar_color, line);
        }

        int GetClientTop() const
        {
            return pixel_scroll.y;
        }

        int GetClientBottom() const
        {
            return pixel_scroll.y + window_rect.size.y;
        }

        vec2 WestToGlobal(const vec2& w) const
        {
            return w + window_rect.position - vec2{0, pixel_scroll.y};
        }

        vec2 ClientToGlobal(const vec2& c)
        {
            const auto cc = c + window_rect.position - pixel_scroll;
            return {cc.x + GetWestWidth(), cc.y};
        }

        vec2 GlobalToClient(const vec2& g)
        {
            const auto cc = vec2{g.x - GetWestWidth(), g.y};
            return cc - window_rect.position + pixel_scroll;
        }

        virtual void DrawWestSide(Painter* painter, const Rect& r) = 0;
        virtual void DrawMainSide(Painter* painter, const Rect& r) = 0;

        bool ShowScrollbarVertical() const
        {
            return true;
        }

        void Draw(Painter* painter) override
        {
            {
                const auto rect = window_rect.CreateWestFromMaxSize(GetWestWidth());
                const auto scope = RectScope(painter, rect);
                DrawWestSide(painter, rect);
            }
            {
                const auto rect = window_rect.CreateEastFromMaxSize(window_rect.size.x - GetWestWidth());
                const auto scope = RectScope(painter, rect);
                DrawMainSide(painter, rect);
            }

            if(ShowScrollbarVertical())
            {
                DrawScrollbarVertical
                (
                    painter
                );
            }
        }

        std::optional<vec2> dragging_offset = std::nullopt;
        bool OnMouseClick(const MouseButton& button, const MouseState state, const vec2& local_mouse, int pixels_to_scroll)
        {
            const auto global_mouse = local_mouse + window_rect.position;
            const auto data = GetVerticalScrollbarData();
            if(state == MouseState::Up && button==MouseButton::Left)
            {
                dragging_offset = std::nullopt;
                return true;
            }

            if(data.rect.Contains(global_mouse) == false)
            {
                return false;
            }

            if(button != MouseButton::Left) { return true; }

            if(data.up_button.Contains(global_mouse))
            {
                ScrollDownPixels(-pixels_to_scroll);
            }
            else if(data.down_button.Contains(global_mouse))
            {
                ScrollDownPixels(pixels_to_scroll);
            }
            else if(data.scrollbar.Contains(global_mouse))
            {
                dragging_offset = data.scrollbar.position - global_mouse;
                OnMouseMoved(local_mouse);
            }

            // todo(Gustav): how shold we scroll when we click outside scrollbar?
            // towards the spot makes sense but by how much?

            return true;
        }

        bool OnMouseMoved(const vec2& local_mouse)
        {
            if(dragging_offset.has_value() == false) { return false; }
            const auto data = GetVerticalScrollbarData();
            const auto global_mouse = local_mouse + window_rect.position;
            const auto new_scrollbar_pos = *dragging_offset + global_mouse;
            const auto pixel_offset = -(data.rect.position.y - new_scrollbar_pos.y);
            // remove button size since the scrollbar is offset by that much
            // not sure where +1 comes from, but without it we scroll automatically when clicking the scrollbar by one pixel
            const auto scroll_factor = static_cast<float>(pixel_offset - data.up_button.size.y + 1)/static_cast<float>(data.area_to_scroll);
            const auto scroll_y = static_cast<int>(scroll_factor * static_cast<float>(data.max_scroll));

            pixel_scroll.y = scroll_y;
            LimitScroll();
            ViewChanged();

            return true;
        }

        float stored_yscroll = 0.0f;
        void OnScrollEvent(float yscroll, int lines, int line_height)
        {
            // todo(Gustav): custom scroll speed lines/multiplier
            stored_yscroll -= yscroll * static_cast<float>(lines * line_height);
            const auto remainder = std::fmod(stored_yscroll, 1.0f);
            const auto pixels = static_cast<int>(stored_yscroll - remainder);
            stored_yscroll = remainder;
            if(pixels != 0)
            {
                ScrollDownPixels(pixels);
            }
        }
    };


    struct Edit
    {
        std::shared_ptr<Settings> settings;
        std::shared_ptr<Font> font;
        Rect rect;
        std::function<void (void)> on_activated;

        std::string text;
        int cursor_from = 0;
        int cursor_to = 0;

        Edit
        (
            std::shared_ptr<Settings> s,
            std::shared_ptr<Font> f
        )
            : settings(s)
            , font(f)
            , rect(EmptyRect)
        {
        }

        void Draw(Painter* painter)
        {
            const auto scope = RectScope(painter, rect);
            painter->Rect(rect, settings->theme.edit_background, std::nullopt);

            const auto start_x = rect.position.x;
            const auto start_y = rect.position.y;

            // todo(Gustav): handle utf8 escape characters when calculating x
            const auto index2x = [this, start_x](int index) -> int { return start_x + font->char_width * index;};

            if(cursor_from != cursor_to)
            {
                const auto cursor_start = std::min(cursor_from, cursor_to);
                const auto cursor_end = std::max(cursor_from, cursor_to);
                const auto select_start = index2x(cursor_start);
                const auto select_end = index2x(cursor_end);
                painter->Rect({{select_start, start_y}, {select_end - select_start, font->line_height}}, settings->theme.edit_selection_bkg, std::nullopt);
            }
            else
            {
                painter->Line({index2x(cursor_from), start_y}, {index2x(cursor_from), start_y + font->line_height}, {settings->theme.edit_cursor, 1});
            }

            painter->Text(font, text, {start_x, start_y}, settings->theme.edit_text);
        }

        bool OnKey(Key key, const Meta& meta)
        {
            switch(key)
            {
                case Key::Return: SelectAll(); on_activated(); break;
                case Key::Left:  cursor_to = StepCursor(cursor_to, -1); if(meta.shift == false) {cursor_from = cursor_to;} break;
                case Key::Right: cursor_to = StepCursor(cursor_to, +1); if(meta.shift == false) {cursor_from = cursor_to;} break;
                case Key::Delete: if(cursor_to != cursor_from) { DeleteToEmptySelection(); } else { if(cursor_from != C(text.length())) { Delete(cursor_from+1);} } break;
                case Key::Backspace: if(cursor_to != cursor_from) { DeleteToEmptySelection(); } else { Delete(cursor_from); cursor_from = cursor_to = StepCursor(cursor_from, -1); } break;
                case Key::Escape: if(text.empty()) {on_activated();} else { cursor_to = cursor_from = 0; text = ""; } break;
                default: return false;
            }

            return true;
        }

        void SelectAll()
        {
            cursor_from = 0;
            cursor_to = C(text.length());
        }

        int StepCursor(int cursor, int steps)
        {
            // todo(Gustav): handle utf8 escape codes when stepping
            // todo(Gustav): handle ctrl stepping over words
            return KeepWithin(0, cursor+steps, C(text.length()));
        }

        void OnChar(const std::string& ch)
        {
            DeleteToEmptySelection();
            text.insert(text.begin() + cursor_from, ch.begin(), ch.end());
            cursor_from = cursor_to = StepCursor(cursor_from, C(ch.length()));
        }

        void Delete(int pos)
        {
            if(pos == 0) { return; }
            const auto at = pos - 1;
            if(text.length() == 0) { return; }
            assert(at < C(text.length()));
            text.erase(text.begin() + at);
        }

        void DeleteToEmptySelection()
        {
            if(cursor_from == cursor_to) { return; }
            const auto cursor_start = std::min(cursor_from, cursor_to);
            const auto cursor_end = std::max(cursor_from, cursor_to);
            text.erase(text.begin() + cursor_start, text.begin() + cursor_end);
            cursor_from = cursor_start;
            cursor_to = cursor_start;
        }

        void OnMouseClick(const vec2&)
        {
            // do selection
        }
    };

    struct CommandView : View
    {
        std::shared_ptr<Settings> settings;
        std::shared_ptr<Font> font;
        Rect rect;

        Edit edit;
        bool enabled = false;

        CommandView
        (
            std::shared_ptr<Settings> s,
            std::shared_ptr<Font> f
        )
            : settings(s)
            , font(f)
            , rect(EmptyRect)
            , edit(settings, font)
        {
            edit.on_activated = [this](){enabled = false; ViewChanged(); std::cout << "Command: " << edit.text << "\n";};
        }

        Rect GetRect() const override
        {
            return rect;
        }

        void Draw(Painter* painter) override
        {
            painter->Rect(rect, settings->theme.command_view_background, std::nullopt);
            edit.Draw(painter);
        }

        void OnKey(Key key, const Meta& meta) override
        {
            if(edit.OnKey(key, meta))
            {
                ViewChanged();
            }
        }

        void OnChar(const std::string& ch) override
        {
            edit.OnChar(ch);
            ViewChanged();
        }

        void OnScroll(float, int) override
        {
        }

        void MouseClick(const MouseButton& button, const MouseState state, const vec2& local_position) override
        {
            if(button != MouseButton::Left) { return; }
            if(state != MouseState::Down) { return; }
            const auto global_position = local_position + rect.position;
            if(edit.rect.Contains(global_position))
            {
                edit.OnMouseClick(global_position);
            }
            else
            {
                enabled = false;
                ViewChanged();
            }
            
        }
        
        void MouseMoved(const vec2&) override
        {
        }
    };


    struct Tab
    {
        std::string name;
        int width;
        int x = 0;

        Tab(const std::string& n, int w) : name(n), width(w) {}
    };

    struct TabsView : public View
    {
        std::shared_ptr<Driver> driver;
        std::shared_ptr<Settings> settings;
        std::shared_ptr<Font> font;
        Rect rect;

        std::vector<Tab> tabs;

        int scroll = 0; // in pixels

        TabsView
        (
            std::shared_ptr<Driver> d,
            std::shared_ptr<Settings> s,
            std::shared_ptr<Font> f
        ) : driver(d), settings(s), font(f), rect(EmptyRect) { }

        int CalculateWidthOfTab(const std::string& name)
        {
            const auto s = driver->GetSizeOfString(font, name);
            const auto total_width = s.width + settings->tab_padding_left + settings->tab_padding_right;
            return total_width;
        }

        void AddFile(const std::string& name)
        {
            const auto start_index = C(tabs.size());
            tabs.emplace_back(name, CalculateWidthOfTab(name));
            UpdateTabPositions(start_index);
            ViewChanged();
        }

        Rect GetRect() const override
        {
            return rect;
        }

        void UpdateTabPositions(int start_tab_index)
        {
            const auto tab_spacing = settings->tab_spacing;
            const auto calculate_x = [tab_spacing](const Tab& t) -> int
            {
                return t.x + t.width + tab_spacing;
            };

            for(int tab_index=start_tab_index; tab_index<C(tabs.size()); tab_index+=1)
            {
                const auto x = (tab_index == 0)
                    ? settings->tab_start_padding
                    : calculate_x(tabs[Cs(tab_index -1)]);
                auto& tab = tabs[Cs(tab_index)];
                tab.x = x;
            }
        }

        int selected_tab = 1;
        
        void Draw(Painter* painter) override
        {
            const auto background_color = settings->theme.tabbar_background_color;
            const auto tab_inactive_background = settings->theme.tabbar_tab_inactive_background;
            const auto tab_selected_background = settings->theme.tabbar_tab_selected_background;
            const auto tab_line_color = settings->theme.tabbar_tab_line_color;
            const auto tab_text_color = settings->theme.tabbar_tab_text_color;
            painter->Rect(rect, background_color, std::nullopt);

            auto scope = RectScope{painter, rect};
            
            const auto bottom = rect.position.y + rect.size.y;
            for
            (
                int tab_index = 0;
                tab_index < C(tabs.size());
                tab_index += 1
            )
            {
                const auto tab = tabs[Cs(tab_index)];
                if(tab.x-scroll > rect.size.x ) { break; }
                const auto is_selected = IsTabIndexSelected(tab_index);
                const auto tab_height_offset = CalculateTabHeightOffset(tab_index);
                const auto tab_background = is_selected ? tab_selected_background : tab_inactive_background;
                const auto tab_rect = CalculateLocalTabRect(tab_index).Offset(rect.position);
                painter->Rect(tab_rect, tab_background, Line{tab_line_color, 1});
                painter->Text(font, tab.name, {tab.x + rect.position.x + settings->tab_padding_left - scroll, bottom - (font->line_height + tab_height_offset)}, tab_text_color);
            }
        }

        bool IsTabIndexSelected(int tab_index) const
        {
            return selected_tab == tab_index;
        }

        Rect CalculateLocalTabRect(int tab_index) const
        {
            const auto bottom = rect.size.y;
            const auto tab = tabs[Cs(tab_index)];
            const auto tab_height = CalculateTabHeight(tab_index);
            return {{tab.x - scroll, bottom - tab_height}, {tab.width, tab_height + 2}};
        }

        // how tall the tab is compared to a regular tab
        int CalculateTabHeightOffset(int tab_index) const
        {
            const auto tab_height_offset = IsTabIndexSelected(tab_index) ? settings->selected_tab_height : 0;
            return tab_height_offset;
        }

        int CalculateTabHeight(int tab_index) const
        {
            const auto tab_height = font->line_height + settings->tab_height_padding + CalculateTabHeightOffset(tab_index);
            return tab_height;
        }

        void OnKey(Key, const Meta&) override
        {
        }

        void OnChar(const std::string&) override
        {
        }

        void OnScroll(float mouse_scroll, int) override
        {
            scroll -= static_cast<int>(mouse_scroll * static_cast<float>(settings->tab_scroll_speed));

            const auto end_of_tab = [](const Tab& t) -> int
            {
                return t.x + t.width;
            };

            const auto scroll_width = tabs.empty() ? 0 : end_of_tab(*tabs.rbegin()) + settings->tab_end_padding;
            const auto max_scroll = std::max(0, scroll_width - rect.size.x);
            scroll = KeepWithin(0, scroll, max_scroll);

            ViewChanged();
        }

        void MouseClick(const MouseButton& button, const MouseState state, const vec2& p) override
        {
            if(button != MouseButton::Left) { return; }
            if(state != MouseState::Down) { return; }

            auto select_tab = [this](int new_tab)
            {
                if(this->selected_tab != new_tab)
                {
                    this->selected_tab = new_tab;
                    this->ViewChanged();
                }
            };

            for(int tab_index=0; tab_index<C(tabs.size()); tab_index+=1)
            {
                // todo(Gustav): does this correctly handle when rect is non-zero?
                const auto tab_rect = CalculateLocalTabRect(tab_index);
                if(tab_rect.Contains(p))
                {
                    select_tab(tab_index);
                    return;
                }
            }

            // todo(Gustav): should we be able to deselect tabs by hitting between or over tabs?
            select_tab(-1);
        }

        void MouseMoved(const vec2&) override
        {
        }
    };


    struct FileSystemView : public View
    {
        std::shared_ptr<Font> font;
        Rect rect;
        std::shared_ptr<Settings> settings;
        std::shared_ptr<FileSystem> filesystem;
        std::string root;

        std::vector<FileEntry> entries;

        FileSystemView
        (
            std::shared_ptr<Font> f,
            std::shared_ptr<Settings> s,
            std::shared_ptr<FileSystem> fs,
            const std::string& rt
        ) : font(f), rect(EmptyRect), settings(s), filesystem(fs), root(rt)
        {
            auto folders_and_files = filesystem->List(root, *settings);
            if(folders_and_files)
            {
                entries = *folders_and_files;
            }
        }

        Rect GetRect() const override
        {
            return rect;
        }
        
        void Draw(Painter* painter) override
        {
            const auto background_color = settings->theme.filesys_background_color;
            const auto folder_color = settings->theme.filesys_folder_color;
            const auto file_color = settings->theme.filesys_file_color;
            const auto hidden_color = settings->theme.filesys_hidden_color;

            painter->Rect(rect, background_color, std::nullopt);
            const auto scope = RectScope{painter, rect};

            int index = 0;
            for(auto p = rect.position; p.y < rect.size.y && index < C(entries.size()); p.y += font->line_height)
            {
                const auto e = entries[Cs(index)]; index +=1;
                const std::string text = e.is_directory ? (Str{} << e.name << "/").ToString() : e.name;
                painter->Text(font, text, p, e.name[0] == '.' ? hidden_color : (e.is_directory ? folder_color : file_color));
            }
        }

        void OnKey(Key, const Meta&) override
        {
        }

        void OnChar(const std::string&) override
        {
        }

        void OnScroll(float, int) override
        {
        }

        void MouseClick(const MouseButton&, const MouseState, const vec2&) override
        {
        }

        void MouseMoved(const vec2&) override
        {
        }
    };


    struct TextView : public ScrollableView
    {
        std::shared_ptr<Driver> driver;
        std::shared_ptr<Font> font;
        std::shared_ptr<Document> document;

        int requested_cursor_x = 0;
        vec2 cursor = {0, 0};

        vec2 GetDocumentSize() override
        {
            return
            {
                // todo(Gustav): calculate document width instead of sending -1
                -1,
                (document->GetNumberOfLines()+1) * font->line_height
            };
        }

        int GetWestWidth() override
        {
            const auto line_number_size = GetLineNumberSize();
            
            if(settings == nullptr) { return line_number_size; }

            return settings->left_gutter_padding + line_number_size + settings->right_gutter_padding;
        }

        void OnScroll(float yscroll, int lines) override
        {
            OnScrollEvent(yscroll, lines, font->line_height);
        }

        void FocusCursor()
        {
            const auto cursor_top_point = vec2
            {
                cursor.x * font->char_width + settings->editor_padding_left,
                cursor.y * font->line_height
            };
            const auto scroll_spacing_half_height = settings->scroll_spacing.y * font->line_height;
            const auto scroll_spacing_half_width = settings->scroll_spacing.x * font->char_width;
            const auto scroll_rect = Rect
            {
                {
                    cursor_top_point.x - scroll_spacing_half_width,
                    cursor_top_point.y - scroll_spacing_half_height
                },
                {
                    scroll_spacing_half_width * 2,
                    // use line_height here since we count from top _and_ extra lines is extra lines _below_
                    scroll_spacing_half_height * 2 + font->line_height
                }
            };
            ScrollToRect(scroll_rect);
            ViewChanged();
        }

        void StepRight(int x)
        {
            const int next_x = std::max(0, cursor.x + x);
            if(document && cursor.y < document->GetNumberOfLines())
            {
                cursor.x = std::min<int>(C(document->GetLineAt(cursor.y).length()), next_x);
            }
            requested_cursor_x = cursor.x;

            FocusCursor();
            ViewChanged();
        }

        // moves a cursor with virtual whitespace to a cursor without virtual whitespace
        vec2 VirtualCursorToActualCursor(const vec2& r) const
        {
            const auto ry = KeepWithin(0, r.y, document->GetNumberOfLines());
            const auto rx = KeepWithin(0, r.x, C(document->GetLineAt(ry).length()));
            return {rx, ry};
        }

        // given a local pixel, get the suggested cursor position
        vec2 LocalPointToCursor(const vec2& pos)
        {
            const auto r = vec2
            {
                static_cast<int>(std::round(static_cast<float>(pos.x) / static_cast<float>(font->char_width))),
                static_cast<int>(std::floor(static_cast<float>(pos.y) / static_cast<float>(font->line_height)))
            };

            return VirtualCursorToActualCursor(r);
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
            cursor = VirtualCursorToActualCursor({requested_cursor_x, cursor.y + y});
            FocusCursor();
            ViewChanged();
        }

        void PlaceCursorAt(const vec2& p)
        {
            cursor = VirtualCursorToActualCursor(p);
            ViewChanged();
        }

        void InsertStringAtCursor(const std::string& str)
        {
            if(document == nullptr) { return; }
            if(cursor.x < 0) { return; }
            if(cursor.y < 0) { return; }

            if(cursor.y == document->GetNumberOfLines())
            {
                document->lines.emplace_back(str);
                cursor.x = C(str.length());
                ViewChanged();
            }
            else
            {
                auto line = document->GetLineAt(cursor.y);
                if(C(line.length()) < cursor.x )
                {
                    // add spaces
                    const auto number_of_spaces = cursor.x - C(line.length());
                    line = line + std::string(Cs(number_of_spaces), ' ');
                }
                line.insert(Cs(cursor.x), str);
                document->lines[Cs(cursor.y)] = line;
                cursor.x += C(str.length());
                ViewChanged();
            }
        }

        int GetLineNumberTop()
        {
            int top = GetClientTop();
            const auto line_top = static_cast<int>(std::floor(static_cast<float>(top) / static_cast<float>(font->line_height)));
            return line_top;
        }
        
        int GetLineNumberBottom()
        {
            int bottom = GetClientBottom();
            const auto line_bottom = static_cast<int>(std::ceil(static_cast<float>(bottom) / static_cast<float>(font->line_height)));
            return line_bottom;
        }

        int LineNumberToY(int line) const
        {
            return line * font->line_height;
        }

        void DrawWestSide(Painter* painter, const Rect& gutter_rect) override
        {
            painter->Rect(gutter_rect, settings->theme.text_gutter_color, std::nullopt);

            if(settings->render_linenumber)
            {
                const auto top = GetLineNumberTop();
                const auto bottom = GetLineNumberBottom();

                for(auto line_index=top; line_index<=bottom; line_index +=1)
                {
                    const auto y = LineNumberToY(line_index);
                    painter->Text(font, Str{} << line_index + 1, WestToGlobal({settings->left_gutter_padding, y}), settings->theme.text_linenumber_color);
                }
            }
        }

        void DrawMainSide(Painter* painter, const Rect& rect) override
        {
            const auto foreground_color = settings->theme.text_foreground_color;
            const auto background_color = settings->theme.text_background_color;

            const auto top = GetLineNumberTop();
            const auto bottom = GetLineNumberBottom();

            painter->Rect(rect, background_color, std::nullopt);

            for(auto line_index=top; line_index<=bottom; line_index +=1)
            {
                const auto y = LineNumberToY(line_index);
                const auto line = document->GetLineAt(line_index);

                painter->Text(font, line, ClientToGlobal({settings->editor_padding_left, y}), foreground_color);

                if(line_index == cursor.y)
                {
                    const auto cursor_x = settings->editor_padding_left + driver->GetSizeOfString(font, line.substr(0, Cs(cursor.x))).width;
                    painter->Line
                    (
                        ClientToGlobal({cursor_x, y}),
                        ClientToGlobal({cursor_x, y + font->line_height}),
                        {settings->theme.text_cursor, 1}
                    );
                }
            }
        }

        TextView
        (
            std::shared_ptr<Driver> d,
            std::shared_ptr<Font> f,
            std::shared_ptr<Document> doc,
            std::shared_ptr<Settings> s
        ) : ScrollableView(s), driver(d), font(f), document(doc) {}

        
        DocumentInformation GetCurrentDocumentInformation() const
        {
            return
            {
                cursor.y + 1,
                cursor.x + 1,
                document->GetNumberOfLines()
            };
        }

        Rect GetRect() const override
        {
            return window_rect;
        }

        void OnKey(Key key, const Meta& meta) override
        {
            const auto ctrl = meta.ctrl;

            bool handled = true;

            switch(key)
            {
            case Key::Left:
                if(ctrl) { ScrollRightPixels(-font->char_width); }
                else { StepRight(-1); }
                break;
            case Key::Right:
                if(ctrl) { ScrollRightPixels(font->char_width); }
                else { StepRight(1); }
                break;
            case Key::Up:
                if(ctrl) { ScrollDownPixels(-font->line_height); }
                else { StepDown(-1); }
                break;
            case Key::Down:
                if(ctrl) { ScrollDownPixels(font->line_height); }
                else { StepDown(1); }
                break;
            default:
                handled = false;
                break;
            }

            if(handled) { ViewChanged(); }
        }

        void OnChar(const std::string& ch) override
        {
            InsertStringAtCursor(ch);
            ViewChanged();
        }

        void MouseClick(const MouseButton& button, const MouseState state, const vec2& local_position) override
        {
            if( OnMouseClick(button, state, local_position, settings->lines_to_scroll_for_scrollbar_button * font->line_height) == true ) { return; }
            if( button != MouseButton::Left) { return; }
            if(state != MouseState::Down) { return; }

            const auto global_position = local_position + GetRect().position;
            const auto client_position = GlobalToClient(global_position);
            const auto dropped_spacing = vec2{client_position.x - settings->editor_padding_left, client_position.y};

            const auto new_pos = LocalPointToCursor(dropped_spacing);
            PlaceCursorAt(new_pos);

            if(settings && settings->scroll_to_cursor_on_click)
            {
                FocusCursor();
            }
        }

        void MouseMoved(const vec2& p) override
        {
            if(OnMouseMoved(p) == true) { return; }
        }
    };


    struct RideApp : App
    {
        std::shared_ptr<Driver> driver;

        std::shared_ptr<Font> font_ui;
        std::shared_ptr<Font> font_code;
        std::shared_ptr<Font> font_big;

        std::shared_ptr<Document> document;
        std::shared_ptr<Settings> settings;

        TextView edit_widget;
        StatusBar statusbar;
        FileSystemView fs_widget;
        TabsView tabs;
        CommandView command_view;

        View* active_widget;

        vec2 window_size = vec2{0,0};

        RideApp(std::shared_ptr<Driver> d, std::shared_ptr<FileSystem> fs, const std::string& root)
            : driver(d)
            , font_code(d->CreateCodeFont(8))
            , document(std::make_shared<Document>())
            , settings(std::make_shared<Settings>())
            , edit_widget(driver, font_code, document, settings)
            , statusbar
                (
                    font_code,
                    settings,
                    [this]()
                    {
                        return edit_widget.GetCurrentDocumentInformation();
                    }
                )
            , fs_widget(font_code, settings, fs, root)
            , tabs(driver, settings, font_code)
            , command_view(settings, font_code)
            , active_widget(&edit_widget)
        {
            for(const auto& f: fs_widget.entries)
            {
                if(f.is_directory == false)
                {
                    tabs.AddFile(f.name);
                }
            }

            command_view.enabled = true;
            for(auto* widget : GetAllViews())
            {
                widget->on_change.Add([this](){this->Refresh();});
            }
            command_view.enabled = false;
        }

        void DoLayout()
        {
            const auto sidebar_width = 160;
            const auto tab_height = 30;

            const auto status_height = statusbar.GetHeight();
            const auto padding = settings->window_padding;

            fs_widget.rect =
            {
                {padding, padding},
                {sidebar_width, window_size.y - (status_height + padding + padding)}
            };
            tabs.rect =
            {
                {sidebar_width + padding + padding, padding},
                {window_size.x - (sidebar_width + padding + padding + padding), tab_height}
            };
            edit_widget.window_rect =
            {
                {sidebar_width + padding + padding, padding + tab_height},
                {window_size.x - (sidebar_width + padding + padding + padding), window_size.y - (status_height + padding + tab_height + padding)}
            };

            command_view.rect =
                Rect{{0, 0}, window_size}
                .CreateNorthFromMaxSize(settings->commandview_height)
                .CreateFromCenterMaxSize(settings->commandview_width)
                ;
            command_view.edit.rect =
                command_view.rect
                .Inset(settings->commandview_edit_inset)
                .CreateNorthFromMaxSize(command_view.edit.font->line_height + settings->commandview_edit_extra_height)
                ;
        }

        void OnSize(const vec2& new_size) override
        {
            window_size = new_size;
            DoLayout();

            Refresh();
        }

        void Refresh()
        {
            driver->Refresh();
        }

        void OnPaint(Painter* painter) override
        {
            if(settings->window_padding != 0)
            {
                painter->Rect({{0, 0}, window_size}, settings->theme.background, std::nullopt);
            }

            edit_widget.Draw(painter);
            fs_widget.Draw(painter);
            tabs.Draw(painter);
            statusbar.Draw(painter, window_size);

            if(command_view.enabled)
            {
                command_view.Draw(painter);
            }
        }

        std::vector<View*> GetAllViews()
        {
            std::vector<View*> r =
            {
                static_cast<View*>(&edit_widget),
                static_cast<View*>(&fs_widget),
                static_cast<View*>(&tabs)
            };

            if(command_view.enabled)
            {
                r.push_back(&command_view);
            }

            return r;
        }

        View* HitTest(const vec2& p)
        {
            for(auto* w: GetAllViews())
            {
                const Rect r = w->GetRect();
                if(r.Contains(p))
                {
                    return w;
                }
            }

            return nullptr;
        }

        vec2 last_mouse = vec2{0,0};
        void OnMouseMoved(const vec2& new_position) override
        {
            last_mouse = new_position;
            GetActiveOrCmd()->MouseMoved(last_mouse - active_widget->GetRect().position);
        }

        void OnMouseLeftWindow() override
        {
        }

        void OnMouseButton(MouseState state, MouseButton button) override
        {
            if(state == MouseState::Down && button == MouseButton::Left)
            {
                active_widget = HitTest(last_mouse);
            }

            if(GetActiveOrCmd() != nullptr)
            {
                GetActiveOrCmd()->MouseClick(button, state, last_mouse - active_widget->GetRect().position);
            }
        }

        void OnMouseScroll(float scroll, int lines) override
        {
            if(command_view.enabled)
            {
                command_view.OnScroll(scroll, lines);
                return;
            }

            // todo(Gustav): should scroll use active widget or hovering widget? as a option?
            auto* w = HitTest(last_mouse);
            if(w == nullptr) { return; }
            w->OnScroll(scroll, lines);
        }

        bool ctrl = false;
        bool alt = false;
        bool shift = false;
        bool OnKey(bool down, Key key) override
        {
            if(key == Key::Control) { ctrl = down; Refresh(); return true; }
            if(key == Key::Alt) { alt = down; Refresh(); return true; }
            if(key == Key::Shift) { shift = down; Refresh(); return true; }

            if(down)
            {
                if(key == Key::Tab && ctrl)
                {
                    command_view.enabled = true;
                    Refresh();
                    return true;
                }
                else
                {
                    if(GetActiveOrCmd() != nullptr)
                    {
                        GetActiveOrCmd()->OnKey(key, {ctrl, shift, alt});
                    }
                }
            }

            if(ctrl || alt)
            {
                // std::cout << "blocked\n";
                return true;
            }
            else
            {
                return false;
            }
        }

        void OnChar(const std::string& ch) override
        {
            if(GetActiveOrCmd() != nullptr)
            {
                GetActiveOrCmd()->OnChar(ch);
            }
        }

        View* GetActiveOrCmd()
        {
            if(command_view.enabled)
            {
                return &command_view;
            }
            else
            {
                return active_widget;
            }
        }
    };

    std::shared_ptr<App>
    CreateApp(std::shared_ptr<Driver> driver, const Arguments& args)
    {
        std::cout << "Started with " << args.name << "\n";
        for(const auto& a: args.arguments)
        {
            std::cout << " - " << a << "\n";
        }

        auto fs = MakeFs();
        auto root = fs->GetCurrentDirectory();
        if(args.arguments.empty() == false)
        {
            const auto resolved = fs->AsAbsolute(args.arguments[0]);
            if(resolved)
            {
                if(fs->Exists(*resolved).value_or(false))
                {
                    root = resolved;
                }
                else
                {
                    std::cout << "Ignoring path and using current\n";
                }
            }
        }

        if(root.has_value() == false)
        {
            std::cerr << "Unable to find a required root folder\n";
            return nullptr;
        }

        return std::make_shared<RideApp>(driver, fs, *root);
    }
}
