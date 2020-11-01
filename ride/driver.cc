#include "driver.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <functional>
#include <algorithm>

#include <filesystem>
#include <system_error>

#include "ride/str.h"
#include "ride/humane_sort.h"


namespace ride
{
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
    };

    struct Settings
    {
        Theme theme;

        int min_size_of_scrollbar = 3;

        int window_padding = 3;

        vec2 scroll_spacing = {3, 3};

        bool render_linenumber = true;
        int left_gutter_padding = 3;
        int right_gutter_padding = 6;

        int editor_padding_left = 6;

        int statusbar_padding_bottom = 3;
        int statusbar_padding_top = 3;
        int statusbar_padding_right = 6;

        bool scroll_to_cursor_on_click = true;

        bool directories_first = true;
        bool sort_files = true;

        int tab_padding_left = 6;
        int tab_padding_right = 6;
        int tab_height_padding = 3;
        int tab_spacing = 3;
        int tab_start_padding = 3;
        int tab_end_padding = 3;
        int selected_tab_height = 6;

        int tab_scroll_speed = 40;
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

        Callbacks on_change;
        void ViewChanged()
        {
            on_change.Call();
        }
    };


    struct Tab
    {
        std::string name;
        int width;
        int x = 0;

        Tab(const std::string& n, int w) : name(n), width(w) {}
    };

    template<typename T>
    T KeepWithin(T min, T t, T max)
    {
        if(t < min) { return min; }
        else if(t > max) { return max; }
        else { return t; }
    }

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
    };


    struct ScrollableView : public View
    {
        std::shared_ptr<Settings> settings;

        Rect window_rect = EmptyRect;
        vec2 pixel_scroll = {0, 0};

        ScrollableView(std::shared_ptr<Settings> s) : settings(s) {}

        // get the full document area
        virtual vec2 GetDocumentSize() = 0;

        // how wide should the gutter area be?
        virtual int GetWestWidth() = 0;

        // vec2 GlobalToLocal(const vec2& global) const;
        // vec2 LocalToGlobal(const vec2& local) const;

        void LimitScroll()
        {
            const auto document_size = GetDocumentSize();

            if(pixel_scroll.y > (document_size.y - window_rect.size.y))
            {
                pixel_scroll.y = document_size.y - window_rect.size.y;
            }

            if(pixel_scroll.x > (document_size.x - window_rect.size.x))
            {
                pixel_scroll.x = document_size.x - window_rect.size.x;
            }

            if(pixel_scroll.x < 0) { pixel_scroll.x = 0; }
            if(pixel_scroll.y < 0) { pixel_scroll.y = 0; }
        }

        void ScrollDownPixels(int y)
        {
            pixel_scroll.y += y;
            LimitScroll();
        }

        void ScrollRightPixels(int x)
        {
            pixel_scroll.x += x;
            LimitScroll();
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
                if(pixel_scroll.y < 0)
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
                const auto steps = pixel_scroll.x + window_rect.size.x - cursor_right;
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

        static void DrawScrollbarVertical(const Settings& settings, Painter* painter, int scroll, int lines_no_view, int lines_in_view, Rect rect)
        {
            const auto background_color = settings.theme.scrollbar_background_color;
            const auto scrollbar_color = settings.theme.scrollbar_scrollbar_color;
            const auto line_color = settings.theme.scrollbar_line_color;
            const auto button_color = settings.theme.scrollbar_button_color;

            const auto line = Line{line_color, 1};
            const auto button_height = rect.size.x;

            const auto up_button = rect.CreateNorthFromMaxSize(button_height);
            const auto down_button = rect.CreateSouthFromMaxSize(button_height);

            painter->Rect(rect, background_color, line);
            painter->Rect(up_button, button_color, line);
            painter->Rect(down_button, button_color, line);

            const auto size_without_buttons = rect.size.y - (up_button.size.y + down_button.size.y);
            if(size_without_buttons <= 0) { return; }

            const auto suggest_scrollbar_size = (static_cast<float>(lines_in_view)/static_cast<float>(lines_no_view)) * static_cast<float>(size_without_buttons);
            const auto scrollbar_size = std::max(settings.min_size_of_scrollbar, static_cast<int>(std::ceil(suggest_scrollbar_size)));

            const auto area_to_scroll = size_without_buttons - scrollbar_size;

            const auto suggest_scroll_position = (static_cast<float>(scroll) / static_cast<float>(lines_no_view)) * static_cast<float>(area_to_scroll);
            const auto scroll_position = static_cast<int>(std::floor(suggest_scroll_position));

            const auto scrollbar = Rect{{rect.position.x, rect.position.y + scroll_position + up_button.size.y}, {rect.size.x, scrollbar_size}};
            painter->Rect(scrollbar, scrollbar_color, line);
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

            DrawScrollbarVertical
            (
                *settings,
                painter,
                pixel_scroll.y,
                GetDocumentSize().y - window_rect.size.y,
                window_rect.size.y,
                window_rect.CreateEastFromMaxSize(10)
            );
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
                ViewChanged();
            }
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
            return {-1, document->GetNumberOfLines() * font->line_height};
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
            // todo(Gustav): fix this!
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

        vec2 VirtualCursorToActualCursor(const vec2& r) const
        {
            const auto ry = std::min<int>(std::max(0, r.y), document->GetNumberOfLines()+1);
            const auto rx = std::min<int>(std::max(0, r.x), C(document->GetLineAt(ry).length()));
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
            if(state != MouseState::Down) { return; }
            if( button != MouseButton::Left) { return; }

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
            , active_widget(&edit_widget)
        {
            for(const auto& f: fs_widget.entries)
            {
                if(f.is_directory == false)
                {
                    tabs.AddFile(f.name);
                }
            }

            for(auto* widget : GetAllViews())
            {
                widget->on_change.Add([this](){this->Refresh();});
            }
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
        }

        std::vector<View*> GetAllViews()
        {
            return
            {
                static_cast<View*>(&edit_widget),
                static_cast<View*>(&fs_widget),
                static_cast<View*>(&tabs)
            };
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

            if(active_widget != nullptr)
            {
                active_widget->MouseClick(button, state, last_mouse - active_widget->GetRect().position);
            }
        }

        void OnMouseScroll(float scroll, int lines) override
        {
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
                if(active_widget != nullptr)
                {
                    active_widget->OnKey(key, {ctrl, shift, alt});
                }
            }

            if(ctrl || alt)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        void OnChar(const std::string& ch) override
        {
            if(active_widget != nullptr)
            {
                active_widget->OnChar(ch);
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
