#pragma once

#include <array>
#include <vector>
#include <string>
#include <memory>
#include <optional>

#include "core/rng.h"
#include "core/hash.h"
#include "core/units.h"

#include "api/renderer.h"
#include "api/font.h"
#include "api/image.h"

namespace ride::backends::blit
{

constexpr int CELLS_X = 80;
constexpr int CELLS_Y = 50;
constexpr int CELL_SIZE = 96;

struct Ren;

enum CommandType
{
    SET_CLIP,
    DRAW_TEXT,
    DRAW_RECT,
    DRAW_IMAGE
};


struct RenCmd
{
    CommandType type;
    Rect<Px> rect_arg = Rect<Px>(0_px);
    Color color;
    std::shared_ptr<api::Font> font;
    std::shared_ptr<api::Image> image;
    Px tab_width = Px{0};
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

    std::vector<RenCmd> command_buf;
    Rect<Px> screen_rect = Rect<Px>(0_px);

    bool show_debug = false;
    Rng rng;

    Ren* ren;

    RenCache(Ren* r);

    std::vector<Rect<Px>> clip_rect_stack;
    void push_clip_rect(const Rect<Px>& Rect);
    void pop_clip_rect();

    // private
    RenCmd& push_command(CommandType type);
    void update_overlapping_cells(const Rect<Px>& r, unsigned h);

    void set_debug(bool enable);
    void free_font(api::Font* font);
    void set_clip_rect(const Rect<Px>& Rect);
    void draw_rect(Rect<Px> rect, Color color);
    void draw_image(std::shared_ptr<api::Image> image, Px x, Px y, Color color);
    Px draw_text(std::shared_ptr<api::Font> font, const std::string& text, Px x, Px y, Color color);
    void invalidate();
    void begin_frame();
    void end_frame();
};


}

