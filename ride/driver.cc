#include "driver.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <functional>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <map>

#include <filesystem>
#include <system_error>

#include "ride/str.h"
#include "ride/humane_sort.h"


namespace ride
{
    // https://twitter.com/idoccor/status/1314664849276899328
    // https://gist.github.com/dirocco/0e23867d0ea9ab2a23dd6dcaba065f53
    template<typename E> constexpr typename std::underlying_type<E>::type cast_to_base(E e) noexcept
    {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

    template<typename T>
    T KeepWithin(T min, T t, T max)
    {
        assert(min <= max);

        if(t < min) { return min; }
        else if(t > max) { return max; }
        else { return t; }
    }

    char ToLowerChar(char b)
    {
        if(b >= 'A' && b <= 'Z')
        {
            return b - 'A' + 'a';
        }
        return b;
    }

    std::string ToLower(const std::string& str)
    {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ToLowerChar);
        return result;
    }

    std::string TrimRight(const std::string& string_to_trim, const std::string& trim_characters)
    {
        return std::string(string_to_trim).erase(string_to_trim.find_last_not_of(trim_characters) + 1);
    }

    std::string TrimLeft(const std::string& string_to_trim, const std::string& trim_characters)
    {
        return std::string(string_to_trim).erase(0, string_to_trim.find_first_not_of(trim_characters));
    }

    std::string Trim(const std::string& string_to_trim, const std::string& trim_characters = " \t\r\n")
    {
        return TrimRight(TrimLeft(string_to_trim, trim_characters), trim_characters);
    }

    template <typename Out>
    void Split(const std::string& s, char delim, Out result)
    {
        std::stringstream ss(s);
        std::string item;
        while(std::getline(ss, item, delim))
        {
            *(result++) = item;
        }
    }

    std::vector<std::string> Split(const std::string& s, char delim)
    {
        std::vector<std::string> elems;
        Split(s, delim, std::back_inserter(elems));
        return elems;
    }

    struct KeyStringMap
    {
        std::map<std::string, Key> string_to_key;
        std::map<Key, std::string> key_to_string;

        void Bind(Key key, const std::string& name)
        {
            const auto id = ToLower(name);
            assert(key_to_string.find(key) == key_to_string.end());
            assert(string_to_key.find(id) == string_to_key.end());

            string_to_key[id] = key;
            key_to_string[key] = name;
        }

        Key FromString(const std::string& name) const
        {
            const auto id = ToLower(name);
            const auto found_key = string_to_key.find(id);
            if(found_key == string_to_key.end())
            {
                std::cout << "Failed to find key " << name << "\n";
                return Key::None;
            }
            return found_key->second;
        }

        std::string ToString(Key key) const
        {
            const auto found_name = key_to_string.find(key);
            if(found_name == key_to_string.end())
            {
                return "<None>";
            }
            return found_name->second;
        }
    };

    KeyStringMap BuildKeyStringMap()
    {
        KeyStringMap map;
        map.Bind(Key::None, "None");
        map.Bind(Key::Backspace, "Backspace");
        map.Bind(Key::Tab, "Tab");
        map.Bind(Key::Return, "Return");
        map.Bind(Key::Escape, "Escape");
        map.Bind(Key::Space, "Space");
        map.Bind(Key::Delete, "Delete");
        map.Bind(Key::Start, "Start");
        map.Bind(Key::LButton, "LButton");
        map.Bind(Key::RButton, "RButton");
        map.Bind(Key::Cancel, "Cancel");
        map.Bind(Key::MButton, "MButton");
        map.Bind(Key::Clear, "Clear");
        map.Bind(Key::Shift, "Shift");
        map.Bind(Key::Alt, "Alt");
        map.Bind(Key::Control, "Control");
        map.Bind(Key::Menu, "Menu");
        map.Bind(Key::Pause, "Pause");
        map.Bind(Key::Capital, "Capital");
        map.Bind(Key::End, "End");
        map.Bind(Key::Home, "Home");
        map.Bind(Key::Left, "Left");
        map.Bind(Key::Up, "Up");
        map.Bind(Key::Right, "Right");
        map.Bind(Key::Down, "Down");
        map.Bind(Key::Select, "Select");
        map.Bind(Key::Print, "Print");
        map.Bind(Key::Execute, "Execute");
        map.Bind(Key::Snapshot, "Snapshot");
        map.Bind(Key::Insert, "Insert");
        map.Bind(Key::Help, "Help");
        map.Bind(Key::Numpad0, "Numpad0");
        map.Bind(Key::Numpad1, "Numpad1");
        map.Bind(Key::Numpad2, "Numpad2");
        map.Bind(Key::Numpad3, "Numpad3");
        map.Bind(Key::Numpad4, "Numpad4");
        map.Bind(Key::Numpad5, "Numpad5");
        map.Bind(Key::Numpad6, "Numpad6");
        map.Bind(Key::Numpad7, "Numpad7");
        map.Bind(Key::Numpad8, "Numpad8");
        map.Bind(Key::Numpad9, "Numpad9");
        map.Bind(Key::Multiply, "Multiply");
        map.Bind(Key::Add, "Add");
        map.Bind(Key::Separator, "Separator");
        map.Bind(Key::Subtract, "Subtract");
        map.Bind(Key::Decimal, "Decimal");
        map.Bind(Key::Divide, "Divide");
        map.Bind(Key::F1, "F1");
        map.Bind(Key::F2, "F2");
        map.Bind(Key::F3, "F3");
        map.Bind(Key::F4, "F4");
        map.Bind(Key::F5, "F5");
        map.Bind(Key::F6, "F6");
        map.Bind(Key::F7, "F7");
        map.Bind(Key::F8, "F8");
        map.Bind(Key::F9, "F9");
        map.Bind(Key::F10, "F10");
        map.Bind(Key::F11, "F11");
        map.Bind(Key::F12, "F12");
        map.Bind(Key::F13, "F13");
        map.Bind(Key::F14, "F14");
        map.Bind(Key::F15, "F15");
        map.Bind(Key::F16, "F16");
        map.Bind(Key::F17, "F17");
        map.Bind(Key::F18, "F18");
        map.Bind(Key::F19, "F19");
        map.Bind(Key::F20, "F20");
        map.Bind(Key::F21, "F21");
        map.Bind(Key::F22, "F22");
        map.Bind(Key::F23, "F23");
        map.Bind(Key::F24, "F24");
        map.Bind(Key::Numlock, "Numlock");
        map.Bind(Key::Scroll, "Scroll");
        map.Bind(Key::PageUp, "PageUp");
        map.Bind(Key::PageDown, "PageDown");
        map.Bind(Key::NumpadSpace, "NumpadSpace");
        map.Bind(Key::NumpadEnter, "NumpadEnter");
        map.Bind(Key::NumpadF1, "NumpadF1");
        map.Bind(Key::NumpadF2, "NumpadF2");
        map.Bind(Key::NumpadF3, "NumpadF3");
        map.Bind(Key::NumpadF4, "NumpadF4");
        map.Bind(Key::NumpadHome, "NumpadHome");
        map.Bind(Key::NumpadLeft, "NumpadLeft");
        map.Bind(Key::NumpadUp, "NumpadUp");
        map.Bind(Key::NumpadRight, "NumpadRight");
        map.Bind(Key::NumpadDown, "NumpadDown");
        map.Bind(Key::NumpadPageUp, "NumpadPageUp");
        map.Bind(Key::NumpadPageDown, "NumpadPageDown");
        map.Bind(Key::NumpadEnd, "NumpadEnd");
        map.Bind(Key::NumpadBegin, "NumpadBegin");
        map.Bind(Key::NumpadInsert, "NumpadInsert");
        map.Bind(Key::NumpadDelete, "NumpadDelete");
        map.Bind(Key::NupadEqual, "NupadEqual");
        map.Bind(Key::NumpadMultiply, "NumpadMultiply");
        map.Bind(Key::NumpadAdd, "NumpadAdd");
        map.Bind(Key::NumpadSeparator, "NumpadSeparator");
        map.Bind(Key::NumpadSubtract, "NumpadSubtract");
        map.Bind(Key::NumpadDecimal, "NumpadDecimal");
        map.Bind(Key::NumpadDivide, "NumpadDivide");
        map.Bind(Key::WindowLeft, "WindowLeft");
        map.Bind(Key::WindowsRight, "WindowsRight");
        map.Bind(Key::WindowsMenu, "WindowsMenu");
        map.Bind(Key::Special1, "Special1");
        map.Bind(Key::Special2, "Special2");
        map.Bind(Key::Special3, "Special3");
        map.Bind(Key::Special4, "Special4");
        map.Bind(Key::Special5, "Special5");
        map.Bind(Key::Special6, "Special6");
        map.Bind(Key::Special7, "Special7");
        map.Bind(Key::Special8, "Special8");
        map.Bind(Key::Special9, "Special9");
        map.Bind(Key::Special10, "Special10");
        map.Bind(Key::Special11, "Special11");
        map.Bind(Key::Special12, "Special12");
        map.Bind(Key::Special13, "Special13");
        map.Bind(Key::Special14, "Special14");
        map.Bind(Key::Special15, "Special15");
        map.Bind(Key::Special16, "Special16");
        map.Bind(Key::Special17, "Special17");
        map.Bind(Key::Special18, "Special18");
        map.Bind(Key::Special19, "Special19");
        map.Bind(Key::Special20, "Special20");
        map.Bind(Key::ExclamationMark, "!");
        map.Bind(Key::DoubleQuotes, "\"");
        map.Bind(Key::Number, "#");
        map.Bind(Key::Dollar, "$");
        map.Bind(Key::Percent, "%");
        map.Bind(Key::Ampersand, "&");
        map.Bind(Key::SingleQuote, "'");
        map.Bind(Key::OpenParenthesis, "(");
        map.Bind(Key::CloseParenthesis, ")");
        map.Bind(Key::Asterisk, "*");
        map.Bind(Key::Plus, "+");
        map.Bind(Key::Comma, ",");
        map.Bind(Key::Hyphen, "-");
        map.Bind(Key::Dot, ".");
        map.Bind(Key::Slash, "/");
        map.Bind(Key::Zero, "0");
        map.Bind(Key::One, "1");
        map.Bind(Key::Two, "2");
        map.Bind(Key::Three, "3");
        map.Bind(Key::Four, "4");
        map.Bind(Key::Five, "5");
        map.Bind(Key::Six, "6");
        map.Bind(Key::Seven, "7");
        map.Bind(Key::Eight, "8");
        map.Bind(Key::Nine, "9");
        map.Bind(Key::Colon, ":");
        map.Bind(Key::Semicolon, ";");
        map.Bind(Key::LessThan, "<");
        map.Bind(Key::Equals, "=");
        map.Bind(Key::GreaterThan, ">");
        map.Bind(Key::QuestionMark, "?");
        map.Bind(Key::At, "@");
        map.Bind(Key::A, "A");
        map.Bind(Key::B, "B");
        map.Bind(Key::C, "C");
        map.Bind(Key::D, "D");
        map.Bind(Key::E, "E");
        map.Bind(Key::F, "F");
        map.Bind(Key::G, "G");
        map.Bind(Key::H, "H");
        map.Bind(Key::I, "I");
        map.Bind(Key::J, "J");
        map.Bind(Key::K, "K");
        map.Bind(Key::L, "L");
        map.Bind(Key::M, "M");
        map.Bind(Key::N, "N");
        map.Bind(Key::O, "O");
        map.Bind(Key::P, "P");
        map.Bind(Key::Q, "Q");
        map.Bind(Key::R, "R");
        map.Bind(Key::S, "S");
        map.Bind(Key::T, "T");
        map.Bind(Key::U, "U");
        map.Bind(Key::V, "V");
        map.Bind(Key::W, "W");
        map.Bind(Key::X, "X");
        map.Bind(Key::Y, "Y");
        map.Bind(Key::Z, "Z");
        map.Bind(Key::OpeningBracket, "[");
        map.Bind(Key::Backslash, "\\");
        map.Bind(Key::ClosingBracket, "]");
        map.Bind(Key::Caret, "^");
        map.Bind(Key::Underscore, "_");
        map.Bind(Key::GraveAccent, "`");
        map.Bind(Key::OpeningBrace, "{");
        map.Bind(Key::VerticalVar, "|");
        map.Bind(Key::ClosingBrace, "}");
        map.Bind(Key::Tilde, "~");

        return map;
    }

    const KeyStringMap key_string_map = BuildKeyStringMap();

    std::string ToString(const Key key)
    {
        return key_string_map.ToString(key);
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

        Rgb command_view_selected_foreground = {0, 0, 0};
        Rgb command_view_selected_background = {200, 200, 200};
        Rgb command_view_deselected_foreground = {0, 0, 0};
        Rgb command_view_deselected_background = {150, 150, 150};
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

        int results_padding_left = 6;
        int results_border_side = 6;
        int results_padding_top = 6;
        int results_padding_middle = 6;
        int results_internal_padding = 3;

        int filesys_indent = 2;
        int filesys_left_padding = 6;
    };


    struct Callbacks
    {
        using CallbackFunction = std::function<void ()>;

        std::vector<CallbackFunction> callbacks;

        void Add(CallbackFunction&& c)
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


    struct FixedSection
    {
        std::function<int ()> get_size = []() -> int {return 0;};
        std::function<void (Painter*, const Rect&)> on_draw = [](Painter*, const Rect&){};
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
        FixedSection west;

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

        Rect GetRect() const override
        {
            return window_rect;
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
                const auto steps = pixel_scroll.x + (window_rect.size.x - scrollbar_width - west.get_size()) - cursor_right;
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
            return {cc.x + west.get_size(), cc.y};
        }

        vec2 GlobalToClient(const vec2& g)
        {
            const auto cc = vec2{g.x - west.get_size(), g.y};
            return cc - window_rect.position + pixel_scroll;
        }

        bool ShowScrollbarVertical() const
        {
            return true;
        }

        void OnDraw(Painter* painter, std::function<void (const Rect& r)> draw_main)
        {
            {
                const auto rect = window_rect.CreateWestFromMaxSize(west.get_size());
                const auto scope = RectScope(painter, rect);
                west.on_draw(painter, rect);
            }
            {
                const auto west_size = west.get_size();
                const auto scroll_width = GetVerticalScrollbarData().rect.size.x;
                const auto width_including_scroll = window_rect.size.x - west_size;
                const auto rect = window_rect.CreateEastFromMaxSize(width_including_scroll).CreateWestFromMaxSize(width_including_scroll - scroll_width);
                const auto scope = RectScope(painter, rect);
                draw_main(rect);
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


    struct Dialog : public View
    {
        // set to false to remove
        bool enabled = true;

        virtual void OnLayout(const vec2& window_size) = 0;
        virtual void BindOnChange(Callbacks::CallbackFunction&& on_change) = 0;
    };


    struct Entry
    {
        virtual ~Entry() {}

        virtual std::string GetName() const = 0;
    };


    struct EntryList
    {
        virtual ~EntryList() {}

        virtual std::vector<std::shared_ptr<Entry>> FindEntries(const std::string& name) = 0;
    };


    struct Command : public Entry
    {
        using OnExecute = std::function<void ()>;
        std::string id;
        std::string name;
        OnExecute on_execute;

        Command(std::string i, std::string n, OnExecute oe)
            : id(std::move(i))
            , name(std::move(n))
            , on_execute(std::move(oe))
        {
        }

        std::string GetName() const override { return name; }
    };


    struct Keybind
    {
        Key key;
        Meta meta;

        Keybind(Key k, const Meta& m)
            : key(k)
            , meta(m)
        {
        }
    };

    bool operator<(const Keybind& lhs, const Keybind& rhs)
    {
        if(lhs.meta.alt != rhs.meta.alt) { return lhs.meta.alt; }
        if(lhs.meta.shift != rhs.meta.shift) { return lhs.meta.shift; }
        if(lhs.meta.ctrl != rhs.meta.ctrl) { return lhs.meta.ctrl; }
        return cast_to_base(lhs.key) < cast_to_base(rhs.key);
    }


    void SetStringMeta(Meta* meta, const std::string& name_big)
    {
        const auto name = ToLower(name_big);

        if(name == "ctrl" || name == "control")
        {
            meta->ctrl = true;
        }
        else if(name == "shift")
        {
            meta->shift = true;
        }
        else if(name == "alt")
        {
            meta->alt = true;
        }
    }


    Keybind ParseKeybind(const std::string& data)
    {
        const auto split = Split(data, '+');

        auto key = Key::None;
        auto meta = Meta{false, false, false};

        const auto size = C(split.size());
        for(int index=0; index<size; index+=1)
        {
            const auto name = Trim(split[Cs(index)]);
            const auto is_last = index == size-1;
            if(is_last)
            {
                key = key_string_map.FromString(name);
            }
            else
            {
                SetStringMeta(&meta, name);
            }
        }

        return Keybind{key, meta};
    }


    struct CommandWithScore
    {
        std::shared_ptr<Command> command;
        int score;

        CommandWithScore(std::shared_ptr<Command> c, int s)
            : command(c)
            , score(s)
        {
        }
    };

    bool operator<(const CommandWithScore& lhs, const CommandWithScore& rhs)
    {
        return lhs.score > rhs.score;
    }


    // fuzzy match from https://github.com/rxi/lite
    int FuzzyMatch(const std::string& n, const std::string& to_find)
    {
        const char *str = n.c_str();
        const char *ptn = to_find.c_str();

        int score = 0;
        int run = 0;

        while (*str && *ptn)
        {
            while (*str == ' ') { str++; }
            while (*ptn == ' ') { ptn++; }
            if (tolower(*str) == tolower(*ptn))
            {
                score += run * 10 - (*str != *ptn);
                run++;
                ptn++;
            }
            else
            {
                score -= 10;
                run = 0;
            }
            str++;
        }
        
        if (*ptn)
        {
            return 0;
        }

        return score - C(std::strlen(str));
    }


    struct CommandList : public EntryList
    {
        std::vector<std::shared_ptr<Command>> commands;
        std::map<std::string, std::shared_ptr<Command>> id_to_command;

        void Add(std::shared_ptr<Command> command)
        {
            const auto id = ToLower(command->id);
            assert(id_to_command.find(id) == id_to_command.end());

            commands.emplace_back(command);
            id_to_command[id] = command;
        }

        std::shared_ptr<Command> FindCommandById(const std::string& command_id)
        {
            const auto id = ToLower(command_id);
            auto found_command = id_to_command.find(id);
            if(found_command == id_to_command.end())
            {
                assert(false && "failed to find command");
                return nullptr;
            }
            else
            {
                return found_command->second;
            }
        }

        void Add(std::string id, std::string name, Command::OnExecute on_execute)
        {
            Add(std::make_shared<Command>(std::move(id), std::move(name), std::move(on_execute)));
        }

        std::vector<std::shared_ptr<Command>> FuzzyFind(const std::string& text)
        {
            constexpr const bool debug_matching = false;
            std::vector<CommandWithScore> scores;
            if(debug_matching) std::cout << "matching " << text << ":\n";
            for(auto c: commands)
            {
                const auto s = FuzzyMatch(c->name, text);
                if(debug_matching) std::cout << " - " << c->name << " " << s << "\n";
                //if(s > 0)
                {
                    scores.emplace_back(c, s);
                }
            }
            if(debug_matching) std::cout << "Found: " << scores.size() << "\n";
            if(debug_matching) std::cout << "\n";

            std::sort
            (
                scores.begin(),
                scores.end()
            );

            std::vector<std::shared_ptr<Command>> r;
            for(auto c: scores)
            {
                r.emplace_back(c.command);
            }
            return r;
        }

        std::vector<std::shared_ptr<Entry>> FindEntries(const std::string& name) override
        {
            auto ff = FuzzyFind(name);
            std::vector<std::shared_ptr<Entry>> r;
            for(auto f: ff)
            {
                r.emplace_back(f);
            }
            return r;
        }
    };



    struct KeybindList
    {
        // todo(Gustav): support chords
        // todo(Gustav): transform to a vector of commands
        std::map<Keybind, std::shared_ptr<Command>> keybinds;

        std::shared_ptr<CommandList> commands;

        explicit KeybindList(std::shared_ptr<CommandList> c) : commands(c) {}

        void Add(const Keybind& bind, std::shared_ptr<Command> command)
        {
            assert(keybinds.find(bind) == keybinds.end());
            keybinds[bind] = command;
        }

        void Add(const std::string& bind, const std::string& id)
        {
            Add(ParseKeybind(bind), commands->FindCommandById(id));
        }

        bool Perform(Key key, const Meta& meta)
        {
            const auto bind = Keybind{key, meta};
            auto found = keybinds.find(bind);
            if(found == keybinds.end()) { return false; }

            found->second->on_execute();
            return true;
        }
    };


    struct Edit
    {
        std::shared_ptr<Settings> settings;
        std::shared_ptr<Font> font;
        Rect rect;

        std::string text;
        int cursor_from = 0;
        int cursor_to = 0;

        Callbacks on_change;

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

        void ClearText()
        {
            cursor_to = 0;
            cursor_from = 0;
            text = "";
            on_change.Call();
        }

        bool OnKey(Key key, const Meta& meta)
        {
            switch(key)
            {
                case Key::Left:  cursor_to = StepCursor(cursor_to, -1); if(meta.shift == false) {cursor_from = cursor_to;} break;
                case Key::Right: cursor_to = StepCursor(cursor_to, +1); if(meta.shift == false) {cursor_from = cursor_to;} break;
                case Key::Delete: if(cursor_to != cursor_from) { DeleteToEmptySelection(); } else { if(cursor_from != C(text.length())) { Delete(cursor_from+1);} } break;
                case Key::Backspace: if(cursor_to != cursor_from) { DeleteToEmptySelection(); } else { Delete(cursor_from); cursor_from = cursor_to = StepCursor(cursor_from, -1); } break;
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
            on_change.Call();
        }

        void Delete(int pos)
        {
            if(pos == 0) { return; }
            const auto at = pos - 1;
            if(text.length() == 0) { return; }
            assert(at < C(text.length()));
            text.erase(text.begin() + at);
            on_change.Call();
        }

        void DeleteToEmptySelection()
        {
            if(cursor_from == cursor_to) { return; }
            const auto cursor_start = std::min(cursor_from, cursor_to);
            const auto cursor_end = std::max(cursor_from, cursor_to);
            text.erase(text.begin() + cursor_start, text.begin() + cursor_end);
            cursor_from = cursor_start;
            cursor_to = cursor_start;
            on_change.Call();
        }

        void OnMouseClick(const vec2&)
        {
            // do selection
        }
    };


    struct CommandResultsView : ScrollableView
    {
        std::shared_ptr<Driver> driver;
        std::shared_ptr<Font> font;

        std::vector<std::shared_ptr<Entry>> results;

        int cursor = 0;

        CommandResultsView
        (
            std::shared_ptr<Driver> d,
            std::shared_ptr<Font> f,
            std::shared_ptr<Settings> s
        ) : ScrollableView(s), driver(d), font(f)
        {
        }

        std::string GetResultAt(int ii) const
        {
            const auto i = Cs(ii);
            if(i >= results.size()) { return ""; }
            else return results[i]->GetName();
        }

        vec2 GetDocumentSize() override
        {
            return
            {
                // todo(Gustav): calculate document width instead of sending -1
                -1,
                C(results.size()) * font->line_height
            };
        }

        void OnScroll(float yscroll, int lines) override
        {
            OnScrollEvent(yscroll, lines, font->line_height);
        }

        void FocusCursor()
        {
            const auto cursor_top_point = vec2
            {
                0,
                cursor * font->line_height
            };
            const auto scroll_rect = Rect
            {
                cursor_top_point,
                {
                    0,
                    font->line_height
                }
            };
            ScrollToRect(scroll_rect);
            ViewChanged();
        }

        // moves a cursor with virtual whitespace to a cursor without virtual whitespace
        vec2 VirtualCursorToActualCursor(const vec2& r) const
        {
            const auto ry = KeepWithin(0, r.y, C(results.size()));
            const auto rx = 0;
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

        void StepDown(int y)
        {
            cursor = VirtualCursorToActualCursor({0, cursor + y}).y;
            FocusCursor();
            ViewChanged();
        }

        void PlaceCursorAt(const vec2& p)
        {
            cursor = VirtualCursorToActualCursor(p).y;
            ViewChanged();
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
            return settings->results_padding_top + line * (font->line_height + settings->results_padding_middle + settings->results_internal_padding * 2);
        }

        void DrawResults(Painter* painter, const Rect& rect)
        {
            const auto top = GetLineNumberTop();
            const auto bottom = GetLineNumberBottom();

            painter->Rect(rect, settings->theme.command_view_background, std::nullopt);

            for(auto line_index=top; line_index<=bottom; line_index +=1)
            {
                if(line_index < 0 || line_index >= C(results.size())) continue;
                const auto y = LineNumberToY(line_index);
                const auto line = GetResultAt(line_index);

                const auto selected = line_index == cursor;
                const auto foreground_color = selected ? settings->theme.command_view_selected_foreground : settings->theme.command_view_deselected_foreground;
                const auto background_color = selected ? settings->theme.command_view_selected_background : settings->theme.command_view_deselected_background;

                painter->Rect
                (
                    {
                        ClientToGlobal
                        ({
                            settings->results_border_side,
                            y - settings->results_internal_padding
                        }),
                        {
                            rect.size.x - settings->results_border_side * 2,
                            font->line_height + settings->results_internal_padding * 2
                        }
                    },
                    background_color, std::nullopt
                );
                painter->Text(font, line, ClientToGlobal({settings->results_border_side + settings->results_padding_left, y}), foreground_color);
            }
        }

        void Draw(Painter* painter) override
        {
            OnDraw(painter, [this, painter](const Rect& rect){ this->DrawResults(painter, rect); });
        }

        Rect GetRect() const override
        {
            return window_rect;
        }

        void OnKey(Key key, const Meta&) override
        {
            switch(key)
            {
            case Key::Up: MoveDown(-1); break;
            case Key::Down: MoveDown(1); break;
            default: break;
            }
        }

        void MoveDown(int steps)
        {
            if(cursor == 0)
            {
                cursor = steps;
            }
            else
            {
                cursor += steps;
            }

            cursor = (cursor + C(results.size())) % C(results.size());
            ViewChanged();
        }

        std::shared_ptr<Entry> GetSelectedOrNull()
        {
            if(cursor < 0) { return nullptr; }
            else if(cursor >= C(results.size())) { return nullptr; }
            else { return results[Cs(cursor)]; }
        }

        void OnChar(const std::string&) override
        {
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


    struct CommandView : Dialog
    {
        std::shared_ptr<Settings> settings;
        std::shared_ptr<Font> font;
        Rect rect;

        Edit edit;

        CommandResultsView results;
        std::shared_ptr<EntryList> entries;

        std::function<void (std::shared_ptr<Entry>)> run_entry;

        void OnLayout(const vec2& window_size) override
        {
            const auto command_rect =
                Rect{{0, 0}, window_size}
                .CreateNorthFromMaxSize(settings->commandview_height)
                .CreateFromCenterMaxSize(settings->commandview_width)
                ;
            edit.rect = command_rect
                .Inset(settings->commandview_edit_inset)
                .CreateNorthFromMaxSize(edit.font->line_height + settings->commandview_edit_extra_height)
                ;
            const auto command_view_rect = edit.rect.Inset(-settings->commandview_edit_inset);
            rect = command_rect;
            results.window_rect = command_rect
                .CreateSouthFromMaxSize(command_rect.size.y - command_view_rect.size.y)
                // .Inset(settings->commandview_edit_inset)
                ;
        }

        void BindOnChange(Callbacks::CallbackFunction&& on_change_callback) override
        {
            on_change.Add(std::move(on_change_callback));
        }

        void RunCommand(const std::shared_ptr<Entry> command)
        {
            enabled = false;
            run_entry(command);
        }

        void OnClickedOutside()
        {
            enabled = false;
        }

        CommandView
        (
            std::shared_ptr<Driver> d,
            std::shared_ptr<Settings> s,
            std::shared_ptr<Font> f,
            std::shared_ptr<EntryList> e,
            std::function<void (std::shared_ptr<Entry>)> r
        )
            : settings(s)
            , font(f)
            , rect(EmptyRect)
            , edit(settings, font)
            , results(d, font, settings)
            , entries(e)
            , run_entry(r)
        {
            results.on_change.Add([this](){ViewChanged();});
            edit.on_change.Add([this](){UpdateResults(); ViewChanged();});
            UpdateResults();
        }

        void UpdateResults()
        {
            results.results = entries->FindEntries(edit.text);
        }

        Rect GetRect() const override
        {
            return rect;
        }

        void Draw(Painter* painter) override
        {
            painter->Rect(rect, settings->theme.command_view_background, std::nullopt);
            edit.Draw(painter);
            results.Draw(painter);
        }

        void OnKey(Key key, const Meta& meta) override
        {
            switch(key)
            {
            case Key::Return:
                {
                    auto entry = results.GetSelectedOrNull();
                    edit.SelectAll();
                    enabled = false;
                    ViewChanged();
                    this->RunCommand(entry);
                }
                break;
            case Key::Escape:
                if(edit.text.empty() == false)
                {
                    edit.ClearText();
                    ViewChanged();
                }
                else
                {
                    enabled = false;
                    ViewChanged();
                }
                break;
            case Key::Up:
            case Key::Down:
                results.OnKey(key, meta);
                break;
            default:
                if(edit.OnKey(key, meta))
                {
                    ViewChanged();
                }
                break;
            }
        }

        void OnChar(const std::string& ch) override
        {
            edit.OnChar(ch);
        }

        void OnScroll(float delta, int lines) override
        {
            results.OnScroll(delta, lines);
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
            else if(rect.Contains(global_position))
            {
                results.MouseClick(button, state, global_position - results.GetRect().position);
            }
            else
            {
                OnClickedOutside();
                ViewChanged();
            }
            
        }
        
        void MouseMoved(const vec2& p) override
        {
            results.MouseMoved(p);
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

    struct Node
    {
        std::string name;
        std::string path;

        int depth;

        Node(const std::string& n, const std::string p) : name(n), path(p) {}
        virtual ~Node() {}

        bool IsHidden() const
        {
            if(name.empty()) { return false; }
            else { return name[0] == '.'; }
        }

        virtual bool OnClick(bool is_doubleclick, std::shared_ptr<FileSystem> fs, const Settings& settings) = 0;
        virtual void Add(std::vector<Node*>* ret, int depth) = 0;
        virtual Rgb GetColor(const Theme&) const = 0;
    };

    
    struct FileNode : public Node
    {
        explicit FileNode(const FileEntry& f) : Node(f.name, f.path) {}

        Rgb GetColor(const Theme& theme) const override
        {
            return IsHidden() ? theme.filesys_hidden_color : theme.filesys_file_color;
        }

        bool OnClick(bool is_doubleclick, std::shared_ptr<FileSystem>, const Settings&) override
        {
            if(is_doubleclick)
            {
                std::cout << "Open or focus " << path << "\n";
            }
            return false;
        }
        void Add(std::vector<Node*>* ret, int d) override
        {
            ret->emplace_back(this);
            depth = d;
        }
    };

    std::vector<std::shared_ptr<Node>> Create(const std::vector<FileEntry>& files);

    struct DirectoryNode : public Node
    {
        std::string base_name;
        bool is_open = false;

        std::vector<std::shared_ptr<Node>> children;

        explicit DirectoryNode(const FileEntry& f) : Node(f.name, f.path), base_name(Str{} << f.name << "/")
        {
            UpdateName();
        }

        void UpdateName()
        {
            if(is_open) { name = Str{} << "- " << base_name; }
            else        { name = Str{} << "+ " << base_name; }
        }

        Rgb GetColor(const Theme& theme) const override
        {
            return IsHidden() ? theme.filesys_hidden_color : theme.filesys_folder_color;
        }

        bool OnClick(bool, std::shared_ptr<FileSystem> filesystem, const Settings& settings) override
        {
            if(is_open)
            {
                is_open = false;
                children.clear();
            }
            else
            {
                is_open = true;
                auto folders_and_files = filesystem->List(path, settings);
                if(folders_and_files)
                {
                    children = Create(*folders_and_files);
                }
            }

            UpdateName();
            return true;
        }
        void Add(std::vector<Node*>* ret, int d) override
        {
            ret->emplace_back(this);
            for(auto c: children)
            {
                c->Add(ret, d+1);
            }
            depth = d;
        }
    };

    std::shared_ptr<Node> Create(const FileEntry& f)
    {
        if(f.is_directory) { return std::make_shared<DirectoryNode>(f); }
        else { return std::make_shared<FileNode>(f); }
    }

    std::vector<std::shared_ptr<Node>> Create(const std::vector<FileEntry>& files)
    {
        std::vector<std::shared_ptr<Node>> entries;

        for(const auto& e: files)
        {
            entries.emplace_back(Create(e));
        }

        return entries;
    }

    struct FileSystemView : public ScrollableView
    {
        std::shared_ptr<Font> font;
        std::shared_ptr<Settings> settings;
        std::shared_ptr<FileSystem> filesystem;
        std::string root;

        std::vector<std::shared_ptr<Node>> roots;
        std::vector<Node*> entries; // stored in roots

        vec2 GetDocumentSize() override
        {
            return
            {
                -1,
                C(entries.size()) * font->line_height
            };
        }

        std::vector<Node*> CreateEntries()
        {
            std::vector<Node*> ret;
            for(auto r: roots)
            {
                r->Add(&ret, 0);
            }
            return ret;
        }

        void Populate()
        {
            entries = CreateEntries();
            ViewChanged();
        }

        FileSystemView
        (
            std::shared_ptr<Font> f,
            std::shared_ptr<Settings> s,
            std::shared_ptr<FileSystem> fs,
            const std::string& rt
        ) : ScrollableView(s), font(f), settings(s), filesystem(fs), root(rt)
        {
            auto folders_and_files = filesystem->List(root, *settings);
            if(folders_and_files)
            {
                roots = Create(*folders_and_files);
            }
            Populate();
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
        
        void Draw(Painter* painter) override
        {
            OnDraw(painter, [this, painter](const Rect& r)
            {
                painter->Rect(r, settings->theme.filesys_background_color, std::nullopt);

                for(auto index = GetLineNumberTop(); index <= GetLineNumberBottom(); index += 1)
                {
                    if(index >= C(entries.size())) { continue; }
                    const auto e = entries[Cs(index)];
                    const auto p = ClientToGlobal({settings->filesys_left_padding + e->depth * settings->filesys_indent * font->char_width, LineNumberToY(index)});
                    painter->Text(font, e->name, p, e->GetColor(settings->theme));
                }
            });
        }

        void OnKey(Key, const Meta&) override
        {
        }

        void OnChar(const std::string&) override
        {
        }

        void OnScroll(float yscroll, int lines) override
        {
            OnScrollEvent(yscroll, lines, font->line_height);
        }

        void MouseClick(const MouseButton& button, const MouseState state, const vec2& local_position) override
        {
            if(OnMouseClick(button, state, local_position, settings->lines_to_scroll_for_scrollbar_button * font->line_height) == true ) { return; }
            if(button != MouseButton::Left) { return; }
            if(state == MouseState::Up) { return; }

            const auto global_position = local_position + GetRect().position;
            const auto client_position = GlobalToClient(global_position);
            const auto index = static_cast<int>(std::floor(static_cast<float>(client_position.y) / static_cast<float>(font->line_height)));

            if(index >= C(entries.size())) { return; }
            auto entry = entries[Cs(index)];

            const auto is_doubleclick = state != MouseState::Down;
            if(entry->OnClick(is_doubleclick, filesystem, *settings))
            {
                Populate();
            }
        }

        void MouseMoved(const vec2& local_mouse) override
        {
            if(OnMouseMoved(local_mouse)) { return; }
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

        int GetGutterWidth()
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

        void DrawGutter(Painter* painter, const Rect& gutter_rect)
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

        void DrawEdit(Painter* painter, const Rect& rect)
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
        ) : ScrollableView(s), driver(d), font(f), document(doc)
        {
            west.get_size = [this]() -> int { return this->GetGutterWidth();};
            west.on_draw = [this](Painter* painter, const Rect& rect) { this->DrawGutter(painter, rect); };
        }

        void Draw(Painter* painter) override
        {
            OnDraw(painter, [this, painter](const Rect& rect){ this->DrawEdit(painter, rect); });
        }

        
        DocumentInformation GetCurrentDocumentInformation() const
        {
            return
            {
                cursor.y + 1,
                cursor.x + 1,
                document->GetNumberOfLines()
            };
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
        
        std::vector<std::shared_ptr<Dialog>> dialogs;

        View* active_widget;

        vec2 window_size = vec2{0,0};
        std::shared_ptr<CommandList> commands = std::make_shared<CommandList>();

        KeybindList keybinds;

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
            , active_widget(&edit_widget)
            , keybinds(commands)
        {
            tabs.AddFile("cat.png");
            tabs.AddFile("dog.md");
            tabs.AddFile("fish.txt");

            for(auto* widget : GetAllViews())
            {
                widget->on_change.Add([this](){this->Refresh();});
            }

            commands->Add
            (
                "core.find-command",
                "Run command",
                [this]()
                {
                    AddDialog
                    (
                        std::make_shared<CommandView>
                        (
                            driver, settings, font_code, commands,
                            [](std::shared_ptr<Entry> entry)
                            {
                                Command* command = static_cast<Command*>(entry.get());
                                command->on_execute();
                            }
                        )
                    );
                    Refresh();
                }
            );
            commands->Add("test.hello", "say hello", []() {std::cout<< "hello\n";});
            commands->Add("test.hi", "shout hi", []() {std::cout<< "hi man!\n";});
            commands->Add("test.compliment", "give compliment", []() {std::cout<< "is nice\n";});
            commands->Add("test.how", "how are you", []() {std::cout<< "am good\n";});
            commands->Add("test.favorite", "favorite animal", []() {std::cout<< "is cat\n";});
            commands->Add("test.other", "other animal", []() {std::cout<< "is dog\n";});
            commands->Add("test.secret", "whisper secret", []() {std::cout<< "...secret\n";});
            commands->Add("test.cat", "cat man", []() {std::cout<< "not a man cat\n";});

            keybinds.Add("ctrl+s", "test.hello");
            keybinds.Add("ctrl+space", "test.cat");
            keybinds.Add("ctrl+tab", "core.find-command");
        }

        void AddDialog(std::shared_ptr<Dialog> dialog)
        {
            dialog->BindOnChange([this](){this->Refresh();});
            dialog->OnLayout(window_size);
            dialogs.emplace_back(dialog);
        }

        void DoLayout()
        {
            const auto sidebar_width = 160;
            const auto tab_height = 30;

            const auto status_height = statusbar.GetHeight();
            const auto padding = settings->window_padding;

            fs_widget.window_rect =
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

            for(auto& d: dialogs)
            {
                d->OnLayout(window_size);
            }
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

            for(auto& d: dialogs)
            {
                d->Draw(painter);
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

            for(auto& d: dialogs)
            {
                r.push_back(d.get());
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

        void ClearClosedDialogs()
        {
            dialogs.erase
            (
                std::remove_if
                (
                    dialogs.begin(), 
                    dialogs.end(),
                    [](std::shared_ptr<Dialog> dialog) -> bool
                    {
                        return dialog->enabled == false;
                    }
                ),
                dialogs.end()
            );
        }

        vec2 last_mouse = vec2{0,0};
        void OnMouseMoved(const vec2& new_position) override
        {
            last_mouse = new_position;
            GetActiveOrCmd()->MouseMoved(last_mouse - active_widget->GetRect().position);
            ClearClosedDialogs();
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
                ClearClosedDialogs();
            }
        }

        std::shared_ptr<Dialog> GetTopDialog()
        {
            if(dialogs.empty()) { return nullptr; }
            return *dialogs.rbegin();
        }

        void OnMouseScroll(float scroll, int lines) override
        {
            auto top = GetTopDialog();
            if(top != nullptr)
            {
                top->OnScroll(scroll, lines);
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
                auto top = GetTopDialog();

                if(top != nullptr)
                {
                    top->OnKey(key, {ctrl, shift, alt});
                    ClearClosedDialogs();
                }
                else
                {
                    const auto meta = Meta{ctrl, shift, alt};
                    if(keybinds.Perform(key, meta))
                    {
                        // aldready done...
                    }
                    else
                    {
                        if(GetActiveOrCmd() != nullptr)
                        {
                            GetActiveOrCmd()->OnKey(key, meta);
                        }
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
                ClearClosedDialogs();
            }
        }

        View* GetActiveOrCmd()
        {
            auto top = GetTopDialog();
            if(top != nullptr)
            {
                // todo(Gustav): change to return shared ptr when we get multi documents
                return top.get();
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
