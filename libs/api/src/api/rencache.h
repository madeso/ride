#pragma once

#include <array>
#include <vector>
#include <string>
#include <memory>
#include <optional>

#include "base/rng.h"
#include "base/hash.h"

#include "api/renderer.h"
#include "api/font.h"
#include "api/image.h"
#include "api/units.h"

constexpr int CELLS_X = 80;
constexpr int CELLS_Y = 50;
constexpr int CELL_SIZE = 96;

enum CommandType
{
    SET_CLIP,
    DRAW_TEXT,
    DRAW_RECT,
    DRAW_IMAGE
};


struct Command
{
    CommandType type;
    rect<dip> rect_arg;
    Color color;
    std::shared_ptr<Font> font;
    std::shared_ptr<Image> image;
    dip tab_width = dip{0};
    std::string text;

    unsigned compute_hash() const;
};

struct CellBuffer
{
    std::array<std::optional<Hash>, CELLS_X * CELLS_Y> data;
};

struct CellBufferRef
{
    CellBuffer* buffer;

    CellBufferRef(CellBuffer* buffer);

    // int idx = cell_idx(x, y); 

    void invalidate_all();
    void reset_single(int x, int y); // [idx] = Hash::INITIAL;
    void update_single(int x, int y, const void* data, std::size_t size); // Hash::update(&cells[idx], &h, sizeof(h));

    static bool is_same(const CellBufferRef& lhs, const CellBufferRef& rhs, int x, int y); // cells[idx] == cells_prev[idx]
};

struct RenCache
{
    CellBuffer cells_buf1;
    CellBuffer cells_buf2;

    CellBufferRef cells_prev = &cells_buf1;
    CellBufferRef cells = &cells_buf2;

    std::vector<Command> command_buf;
    rect<dip> screen_rect;

    bool show_debug = false;
    Rng rng;

    Ren* ren;

    RenCache(Ren* r);

    std::vector<rect<dip>> clip_rect_stack;
    void push_clip_rect(const rect<dip>& rect);
    void pop_clip_rect();

    // private
    Command& push_command(CommandType type);
    void update_overlapping_cells(const rect<dip>& r, unsigned h);

    void set_debug(bool enable);
    void free_font(Font* font);
    void set_clip_rect(const rect<dip>& rect);
    void draw_rect(rect<dip> rect, Color color);
    void draw_image(std::shared_ptr<Image> image, dip x, dip y, Color color);
    dip draw_text(std::shared_ptr<Font> font, const std::string& text, dip x, dip y, Color color);
    void invalidate();
    void begin_frame();
    void end_frame();
};

struct ClipScope
{
    RenCache* cache;

    ClipScope(RenCache* c, const rect<dip>& r);
    ~ClipScope();
};
