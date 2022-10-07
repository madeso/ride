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

pix get_x_position(Theme* theme, int depth)
{
    return theme->filesys_left_padding + static_cast<double>(depth) * theme->filesys_indent;
}

void ViewFilesystem::update_rects_for_entries()
{
    body_width = 0_px;
    for(std::size_t index = 0; index < entries.size(); index += 1)
    {
        auto* e = entries[index];
        const auto xp = get_x_position(theme, e->depth);

        const auto width = app->to_pix(font->get_width(e->name));
        const auto total_width = width + xp;

        if(total_width > body_width)
        {
            body_width = total_width;
        }
    }
}

void ViewFilesystem::on_layout_body()
{
    update_rects_for_entries();
}

void ViewFilesystem::recreate_entries_list()
{
    node_hovering = std::nullopt;
    
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

pix ViewFilesystem::get_document_width() const
{
    return body_width;
}

std::size_t ViewFilesystem::get_number_of_lines() const
{
    return entries.size();
}


void ViewFilesystem::draw_body(Renderer* cache)
{
    draw_rect(cache, app->to_dip(body_rect), theme->filesys_background_color);

    if(node_hovering)
    {
        draw_rect(cache, app->to_dip(hit_rect_for_line(line_number_to_y(*node_hovering)).get_offset(scroll)), theme->filesys_hover_color);
        cursor = cursor_type::hand;
    }
    else
    {
        cursor = cursor_type::arrow;
    }
    
    draw_lines(cache);
}


void ViewFilesystem::draw_line(Renderer* cache, std::size_t index, const pix& x, const pix& y)
{
    const auto& e = entries[index];
    const auto xp = get_x_position(theme, e->depth);
    draw_text
    (
        cache,
        font,
        e->name,
        app->to_dip(x + xp),
        app->to_dip(y),
        e->get_text_color(*theme)
    );
}


void ViewFilesystem::on_mouse_pressed(MouseButton button, const Meta&, const vec2<pix>& new_mouse, int clicks)
{
    if(button != MouseButton::left) { return; }
    // if(clicks > 2) { return; } // file specific

    if(auto node_index = get_index_under_view_position(new_mouse); node_index)
    {
        if(entries[*node_index]->on_click(clicks == 2, fs, *theme))
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
    node_hovering = get_index_under_view_position(last_mouse);
}
