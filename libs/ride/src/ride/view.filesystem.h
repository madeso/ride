#pragma once

#include <memory>
#include <vector>

#include "base/units.h"
#include "base/color.h"

#include "ride/view.h"

struct filesystem;

struct Node
{
    std::string name;
    std::string path;
    int depth;

    Node(const std::string& n, const std::string& p);
    virtual ~Node();

    bool is_hidden() const;

    virtual bool on_click(bool is_doubleclick, filesystem* fs, const Theme& theme) = 0;
    virtual void add_to_list(std::vector<Node*>* ret, int depth) = 0;
    virtual Color get_text_color(const Theme&) const = 0;
};



struct ViewFilesystem : public LineView
{
    ViewFilesystem();

    filesystem* fs;
    std::string root;
    std::vector<std::shared_ptr<Node>> roots;
    std::vector<Node*> entries; // stored in roots
    std::optional<std::size_t> node_hovering;
    vec2<pix> last_mouse = {0_px, 0_px};
    pix body_width = 0_px;

    void setup();

    void update_rects_for_entries();
    void recreate_entries_list();
    void update_hover();

    void on_layout_body() override;

    void draw_body(Renderer* cache) override;
    void on_mouse_pressed(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse, int clicks) override;
    void on_mouse_moved(const Meta& meta, const vec2<pix>& new_mouse) override;


    void draw_line(Renderer* cache, std::size_t index, const pix& x, const pix& y) override;
    pix get_document_width() const override;
    std::size_t get_number_of_lines() const override;
};
