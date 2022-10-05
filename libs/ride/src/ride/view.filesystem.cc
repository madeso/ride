#include "ride/view.filesystem.h"

#include <iostream>

// #include "fmt/format.h"
// #include "fmt/core.h"
// #include "fmt/ostream.h"
// using namespace fmt::literals;

#include "base/str.h"
#include "base/filesystem.h"

#include "api/app.h"
#include "api/renderer.h"

#include "ride/theme.h"


std::vector<std::shared_ptr<Node>> create_view_entries(const std::vector<FileEntry>& files);

ListSettings create_list_settings_from_theme(const Theme& theme)
{
    return {theme.sort_files, theme.directories_first};
}


///////////////////////////////////////////////////////////////////////////////////////////////////


Node::Node(const std::string& n, const std::string& p)
    : name(n)
    , path(p)
    , position{0_px, 0_px}
    , depth{0}
{}


Node::~Node() = default;


bool Node::is_hidden() const
{
    if(name.empty()) { return false; }
    else { return name[0] == '.'; }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


struct FileNode : public Node
{
    explicit FileNode(const FileEntry& f) : Node(f.name, f.path) {}

    Color get_text_color(const Theme& theme) const override
    {
        return is_hidden() ? theme.filesys_hidden_color : theme.filesys_file_color;
    }

    bool on_click(bool is_doubleclick, filesystem*, const Theme&) override
    {
        if(is_doubleclick)
        {
            std::cout << "Open or focus " << path << "\n";
        }
        return false;
    }
    void add_to_list(std::vector<Node*>* ret, int d) override
    {
        ret->emplace_back(this);
        depth = d;
    }
};


///////////////////////////////////////////////////////////////////////////////////////////////////


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

    Color get_text_color(const Theme& theme) const override
    {
        return is_hidden() ? theme.filesys_hidden_color : theme.filesys_folder_color;
    }

    bool on_click(bool, filesystem* filesystem, const Theme& theme) override
    {
        if(is_open)
        {
            is_open = false;
            children.clear();
        }
        else
        {
            is_open = true;
            auto folders_and_files = filesystem->list(path, create_list_settings_from_theme(theme));
            if(folders_and_files)
            {
                children = create_view_entries(*folders_and_files);
            }
        }

        UpdateName();
        return true;
    }

    void add_to_list(std::vector<Node*>* ret, int d) override
    {
        ret->emplace_back(this);
        for(auto c: children)
        {
            c->add_to_list(ret, d+1);
        }
        depth = d;
    }
};


///////////////////////////////////////////////////////////////////////////////////////////////////


std::shared_ptr<Node> create_view_entry(const FileEntry& f)
{
    if(f.is_directory) { return std::make_shared<DirectoryNode>(f); }
    else { return std::make_shared<FileNode>(f); }
}


std::vector<std::shared_ptr<Node>> create_view_entries(const std::vector<FileEntry>& files)
{
    std::vector<std::shared_ptr<Node>> entries;

    for(const auto& e: files)
    {
        entries.emplace_back(create_view_entry(e));
    }

    return entries;
}


///////////////////////////////////////////////////////////////////////////////////////////////////

ViewFilesystem::ViewFilesystem()
{
    cursor = cursor_type::hand;
}


void ViewFilesystem::update_rects_for_entries()
{
    body_width = 0_px;
    for(std::size_t index = 0; index < entries.size(); index += 1)
    {
        auto* e = entries[index];
        const auto p = vec2<pix>
        {
            theme->filesys_left_padding + static_cast<double>(e->depth) * theme->filesys_indent,
            line_number_to_y(index)
        };

        const auto width = app->to_pix(font->get_width(e->name));
        const auto total_width = width + p.x;

        if(total_width > body_width)
        {
            body_width = total_width;
        }

        e->position = p;
    }
}

void ViewFilesystem::on_layout_body()
{
    update_rects_for_entries();
}

void ViewFilesystem::recreate_entries_list()
{
    node_hovering = nullptr;
    
    entries = {};
    for(auto r: roots)
    {
        r->add_to_list(&entries, 0);
    }

    update_rects_for_entries();
    update_hover();
    // ViewChanged();
}


void ViewFilesystem::setup()
{
    auto folders_and_files = fs->list(root, create_list_settings_from_theme(*theme));
    if(folders_and_files)
    {
        roots = create_view_entries(*folders_and_files);
    }
    recreate_entries_list();
}


pix ViewFilesystem::calculate_line_height() const
{
    return app->to_pix(font->get_height()) + theme->line_spacing;
}


pix ViewFilesystem::line_number_to_y(std::size_t line) const
{
    const auto line_offset = static_cast<double>(line) * calculate_line_height();
    return client_rect.height - calculate_line_height() - line_offset;
}


ScrollSize ViewFilesystem::calculate_scroll_size()
{
    return
    {
        // todo(Gustav): is this correct?
        body_width,
        static_cast<double>(entries.size()) * calculate_line_height()
    };
}


rect<pix> ViewFilesystem::hit_rect_for_node(Node* node)
{
    assert(node);
    const auto height = app->to_pix(font->get_height());
    const auto spacing = theme->line_spacing;

    return rect<pix>::from_ltrb
    (
        body_rect.get_left(),
        node->position.y + height + spacing,
        body_rect.get_right(),
        node->position.y
    );
}

void ViewFilesystem::draw_body(Renderer* cache)
{
    draw_rect(cache, app->to_dip(body_rect), theme->filesys_background_color);

    if(node_hovering)
    {
        draw_rect(cache, app->to_dip(hit_rect_for_node(node_hovering).get_offset(scroll)), theme->filesys_hover_color);
        cursor = cursor_type::hand;
    }
    else
    {
        cursor = cursor_type::arrow;
    }
    

    for(const auto& e: entries)
    {
        draw_text
        (
            cache,
            font,
            e->name,
            app->to_dip(body_rect.x + e->position.x + scroll.x),
            app->to_dip(body_rect.y + e->position.y + scroll.y),
            e->get_text_color(*theme)
        );
    }
}


Node* ViewFilesystem::get_node_under_cursor(const vec2<pix> relative_mouse)
{
    const auto p = vec2<pix>{relative_mouse.x - scroll.x, relative_mouse.y - scroll.y};

    // todo(Gustav): guesstimate entry from y coordinate and then do the checks to avoid checking all the items...
    for(const auto& e: entries)
    {
        if(hit_rect_for_node(e).contains(p))
        {
            return e;
        }
    }

    return nullptr;
}


void ViewFilesystem::on_mouse_pressed(MouseButton button, const Meta&, const vec2<pix>& new_mouse, int clicks)
{
    if(button != MouseButton::left) { return; }
    // if(clicks > 2) { return; } // file specific

    auto* node = get_node_under_cursor(new_mouse);
    if(node)
    {
        if(node->on_click(clicks == 2, fs, *theme))
        {
            recreate_entries_list();
        }
    }
}


void ViewFilesystem::on_mouse_moved(const Meta&, const vec2<pix>& new_mouse)
{
    last_mouse = new_mouse;
    update_hover();
}


void ViewFilesystem::update_hover()
{
    node_hovering = get_node_under_cursor(last_mouse);
}
