#pragma once

#include <vector>
#include <string>
#include <memory>
#include <random>

#include "api/renderer.h"
#include "api/font.h"

constexpr int CELLS_X = 80;
constexpr int CELLS_Y = 50;
constexpr int CELL_SIZE = 96;

enum CommandType
{
    SET_CLIP,
    DRAW_TEXT,
    DRAW_RECT
};

struct Rng
{
    std::default_random_engine engine;

    static std::default_random_engine create();

    Rng();

    template <typename T>
    T generate(T max = std::numeric_limits<T>::max(), T min = std::numeric_limits<T>::min())
    {
        std::uniform_int_distribution<int> uniform_dist(min, max);
        return uniform_dist(engine);
    }
};

struct Command
{
    CommandType type;
    Rect rect;
    Color color;
    std::shared_ptr<Font> font;
    int tab_width;
    std::string text;

    unsigned compute_hash() const;
};

struct RenCache
{
    unsigned cells_buf1[CELLS_X * CELLS_Y];
    unsigned cells_buf2[CELLS_X * CELLS_Y];

    unsigned* cells_prev = cells_buf1;
    unsigned* cells = cells_buf2;

    std::vector<Command> command_buf;
    Rect screen_rect;

    bool show_debug = false;
    Rng rng;

    Ren* ren;

    RenCache(Ren* r);

    // private
    Command& push_command(CommandType type);
    void update_overlapping_cells(Rect r, unsigned h);

    void set_debug(bool enable);
    void free_font(Font* font);
    void set_clip_rect(Rect rect);
    void draw_rect(Rect rect, Color color);
    int draw_text(std::shared_ptr<Font> font, const char* text, int x, int y, Color color);
    void invalidate();
    void begin_frame();
    void end_frame();
};
