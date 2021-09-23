#include <iostream>

#include "base/str.h"
#include "base/cc.h"
#include "base/filesystem.h"

#include "api/app.h"
#include "api/main.h"
#include "api/rencache.h"
#include "api/image.h"
#include "api/font.h"

#include "libride/document.h"

#include "ride/theme.h"
#include "ride/view.h"

#include "logo_256text_png.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

ListSettings list_settings_from_theme(const Theme& theme)
{
    return {theme.sort_files, theme.directories_first};
}

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

    virtual bool OnClick(bool is_doubleclick, filesystem* fs, const Theme& theme) = 0;
    virtual void Add(std::vector<Node*>* ret, int depth) = 0;
    virtual Color GetColor(const Theme&) const = 0;
};


struct FileNode : public Node
{
    explicit FileNode(const FileEntry& f) : Node(f.name, f.path) {}

    Color GetColor(const Theme& theme) const override
    {
        return IsHidden() ? theme.filesys_hidden_color : theme.filesys_file_color;
    }

    bool OnClick(bool is_doubleclick, filesystem*, const Theme&) override
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

    Color GetColor(const Theme& theme) const override
    {
        return IsHidden() ? theme.filesys_hidden_color : theme.filesys_folder_color;
    }

    bool OnClick(bool, filesystem* filesystem, const Theme& theme) override
    {
        if(is_open)
        {
            is_open = false;
            children.clear();
        }
        else
        {
            is_open = true;
            auto folders_and_files = filesystem->list(path, list_settings_from_theme(theme));
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

struct ViewFilesystem : public View
{
    std::shared_ptr<Font> font;
    //std::shared_ptr<Theme> theme;
    filesystem* filesystem;
    std::string root;

    std::vector<std::shared_ptr<Node>> roots;
    std::vector<Node*> entries; // stored in roots

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
        // ViewChanged();
    }

    void setup()
    {
        auto folders_and_files = filesystem->list(root, list_settings_from_theme(*theme));
        if(folders_and_files)
        {
            roots = Create(*folders_and_files);
        }
        Populate();
    }

    pix calculate_line_height() const
    {
        return app->to_pix(font->get_height()) + theme->line_spacing;
    }

    pix LineNumberToY(std::size_t line) const
    {
        return static_cast<double>(line) * calculate_line_height();
    }

    scroll_size calculate_scroll_size() override
    {
        return
        {
            std::nullopt,
            static_cast<double>(entries.size()) * calculate_line_height()
        };
    }

    void draw_body(const rect<pix>& main_view_rect, RenCache* cache) override
    {
        cache->draw_rect(app->to_dip(main_view_rect), theme->filesys_background_color);

        for(std::size_t index = 0; index < entries.size(); index += 1)
        {
            const auto e = entries[index];
            const auto p = vec2<pix>
            {
                theme->filesys_left_padding + static_cast<double>(e->depth) * theme->filesys_indent,
                LineNumberToY(index)
            };
            
            cache->draw_text
            (
                font,
                e->name,
                app->to_dip(main_view_rect.x + p.x - scroll.x),
                app->to_dip(main_view_rect.y + p.y - scroll.y),
                e->GetColor(*theme)
            );
        }
    }

    /*

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
    
    

    void MouseClick(const MouseButton& button, const MouseState state, const vec2& local_position) override
    {
        if(OnMouseClick(button, state, local_position, theme->lines_to_scroll_for_scrollbar_button * font->line_height) == true ) { return; }
        if(button != MouseButton::Left) { return; }
        if(state == MouseState::Up) { return; }

        const auto global_position = local_position + GetRect().position;
        const auto client_position = GlobalToClient(global_position);
        const auto index = static_cast<int>(std::floor(static_cast<float>(client_position.y) / static_cast<float>(font->line_height)));

        if(index >= C(entries.size())) { return; }
        auto entry = entries[Cs(index)];

        const auto is_doubleclick = state != MouseState::Down;
        if(entry->OnClick(is_doubleclick, filesystem, *theme))
        {
            Populate();
        }
    }
    */
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

struct ViewDoc : View
{
    Document doc;
    std::shared_ptr<Font> font;

    pix calculate_line_height()
    {
        return  app->to_pix(font->get_height()) + theme->line_spacing;
    }

    pix get_document_height()
    {
        const auto lines = doc.GetNumberOfLines();
        const auto spacing = calculate_line_height();

        // todo(Gustav): fix cast here
        return spacing * static_cast<double>(lines);
    }

    pix get_document_width()
    {
        const auto longest_line = std::max_element
        (
            doc.lines.begin(), doc.lines.end(),
            [](const std::string& lhs, const std::string& rhs) -> bool
            {
                return lhs.length() < rhs.length();
            }
        );

        if(longest_line == doc.lines.end())
        {
            return 0_px;
        }

        return app->to_pix(font->get_width(*longest_line));
    }

    scroll_size calculate_scroll_size() override
    {
        const auto line_height = calculate_line_height();
        
        const auto w = get_document_width();
        const auto h = get_document_height() - line_height;

        return {w, h};
    }

    void draw_body(const rect<pix>& main_view_rect, RenCache* cache) override
    {
        const auto lines = doc.GetNumberOfLines();

        const auto min_gutter_width = app->to_pix(font->get_width( (Str{} << (lines+1)).ToString().c_str() ));
        const auto gutter_width = min_gutter_width + theme->gutter_spacing_left + theme->gutter_spacing_right;

        auto view_rect = main_view_rect;
        const auto gutter_rect = view_rect.cut_left(gutter_width);

        const auto spacing = calculate_line_height();

        cache->draw_rect(app->to_dip(main_view_rect), theme->edit_background);
        cache->draw_rect(app->to_dip(gutter_rect), theme->gutter_background);

        for(int line_index=0; line_index<lines; line_index += 1)
        {
            const auto y = static_cast<double>(line_index) * spacing - scroll.y;

            cache->draw_text
            (
                font,
                (Str{} << line_index+1).ToString(),
                app->to_dip(main_view_rect.x + theme->gutter_spacing_left),
                app->to_dip(main_view_rect.y + y),
                theme->gutter_color
            );

            {
                const auto text_scope = ClipScope(cache, app->to_dip(view_rect));
                cache->draw_text
                (
                    font,
                    doc.GetLineAt(line_index),
                    app->to_dip(main_view_rect.x + gutter_width + theme->text_spacing - scroll.x),
                    app->to_dip(main_view_rect.y + y),
                    theme->plain_text_color
                );
            }
        }
    }
};


struct RideApp : App
{
    vec2<pix> mouse = {pix{0}, pix{0}};

    std::shared_ptr<Image> logo;
    std::shared_ptr<Font> font;
    std::unique_ptr<filesystem> fs;

    Theme theme;

    ViewDoc root;
    ViewFilesystem browser;
    

    void setup_view(View* view)
    {
        view->theme = &theme;
        view->app = this;
    }

    RideApp()
        : logo(load_shared(LOGO_256TEXT_PNG))
        , font(load_font(Font::default_font, pix{12}))
        , fs(create_local_filesystem())
    {
        setup_view(&root);
        root.font = font;
        root.doc.LoadFile(fs.get(), __FILE__);

        setup_view(&browser);
        browser.font = font;
        browser.filesystem = fs.get();
        if(auto cd = fs->get_current_directory(); cd)
        {
            browser.root = *cd;
            browser.setup();
        }
    }

    void on_mouse_moved(const vec2<pix>& new_mouse, pix xrel, pix yrel) override
    {
        mouse = new_mouse;
    }

    void draw(RenCache* cache) override
    {
        auto rect = ::rect<pix>::from_size(client_size);
        cache->draw_rect(to_dip(rect), theme.window_background_color);

        cache->draw_image(logo, to_dip(pix{10}), to_dip(pix{10}), theme.logo_color);

        const auto browser_rect = rect.cut_left(150_px);

        const auto border_size = 3_px;

        browser.set_rect(browser_rect.Inset(border_size));
        root.set_rect(rect.Inset(border_size));

        browser.draw(cache);
        root.draw(cache);

        cache->draw_rect
        (
            to_dip
            (
                ::rect<pix>
                {
                    mouse,
                    ::size<pix>
                    {
                        pix{10},
                        pix{10}
                    }
                }
            ),
            Color::rgb(0, 0, 255, 255)
        );
    }

    void on_mouse_wheel(int dx, int dy) override
    {
        root.on_mouse_wheel(dx, dy);
    }
};

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> { return std::make_unique<RideApp>();});
}

