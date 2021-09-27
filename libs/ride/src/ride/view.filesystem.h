#pragma once

#include <memory>
#include <vector>

#include "api/units.h"
#include "api/font.h"

#include "ride/view.h"

struct filesystem;

struct Node
{
    std::string name;
    std::string path;
    vec2<pix> position;
    int depth;

    Node(const std::string& n, const std::string& p);
    virtual ~Node();

    bool IsHidden() const;

    virtual bool OnClick(bool is_doubleclick, filesystem* fs, const Theme& theme) = 0;
    virtual void Add(std::vector<Node*>* ret, int depth) = 0;
    virtual Color GetColor(const Theme&) const = 0;
};



struct ViewFilesystem : public View
{
    std::shared_ptr<Font> font;
    filesystem* fs;
    std::string root;
    std::vector<std::shared_ptr<Node>> roots;
    std::vector<Node*> entries; // stored in roots
    Node* node_hovering = nullptr;
    vec2<pix> last_mouse = {0_px, 0_px};
    pix body_width = 0_px;

    void setup();

    std::vector<Node*> CreateEntries();
    void update_rects_for_entries();
    void Populate();
    pix calculate_line_height() const;
    pix LineNumberToY(std::size_t line) const;
    rect<pix> hit_rect_for_node(Node* node);
    Node* get_node_under_cursor(const vec2<pix> relative_mouse);
    void update_hover();

    scroll_size calculate_scroll_size() override;
    void draw_body(RenCache* cache) override;
    void on_mouse_pressed(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse, int clicks) override;
    void on_mouse_moved(const vec2<pix>& new_mouse) override;

};
