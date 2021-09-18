#include "api/rencache.h"

#include <cassert>

/* a cache over the software renderer -- all drawing operations are stored as
** commands when issued. At the end of the frame we write the commands to a grid
** of hash values, take the cells that have changed since the previous frame,
** merge them into dirty rectangles and redraw only those regions */

/* 32bit fnv-1a hash */
struct Hash
{
    static constexpr unsigned INITIAL = 2166136261;
    unsigned value = INITIAL;

    void add(const void* data, int size)
    {
        update(&value, data, size);
    }

    static void update(unsigned* value, const void* data, int size)
    {
        const unsigned char* bytes = static_cast<const unsigned char*>(data);
        for (int i = 0; i < size; i += 1)
        {
            *value = (*value ^ bytes[i]) * 16777619;
        }
    }

    template <typename T>
    Hash& operator<<(const T& t)
    {
        add(&t, sizeof(T));
        return *this;
    }
};

std::default_random_engine Rng::create()
{
    std::random_device random_device;
    return std::default_random_engine(random_device());
}

Rng::Rng()
    : engine(create())
{
}

int cell_idx(int x, int y)
{
    return x + y * CELLS_X;
}

void RenCache::push_clip_rect(const Rect& rect)
{
    assert(false && "todo(Gustav): port the lua code");
    // local x2, y2, w2, h2 = *clip_rect_stack.rbegin();
    // local r, b, r2, b2 = x+w, y+h, x2+w2, y2+h2
    // x, y = math.max(x, x2), math.max(y, y2)
    // b, r = math.min(b, b2), math.min(r, r2)
    // w, h = r-x, b-y
    // clip_rect_stack.push_back({ x, y, w, h });
    // set_clip_rect({x, y, w, h});
}

void RenCache::pop_clip_rect()
{
    clip_rect_stack.pop_back();
    const auto top = *clip_rect_stack.rbegin();
    set_clip_rect(top);
}

Command& RenCache::push_command(CommandType type)
{
    command_buf.emplace_back();
    auto& c = *command_buf.rbegin();
    c.type = type;
    return c;
}

void RenCache::set_debug(bool enable)
{
    show_debug = enable;
}

void RenCache::set_clip_rect(const Rect& rect)
{
    Command& cmd = push_command(SET_CLIP);
    cmd.rect = Rect::intersect(rect, screen_rect);
}

void RenCache::draw_rect(Rect rect, Color color)
{
    if (!Rect::overlap(screen_rect, rect))
    {
        return;
    }
    Command& cmd = push_command(DRAW_RECT);
    cmd.rect = rect;
    cmd.color = color;
}

void RenCache::draw_image(std::shared_ptr<Image> image, int x, int y, Color color)
{
    auto rect = Rect
    {
        x,
        y,
        image->width,
        image->height
    };

    if (!Rect::overlap(screen_rect, rect))
    {
        return;
    }
    Command& cmd = push_command(DRAW_IMAGE);
    cmd.rect = rect;
    cmd.image = image;
    cmd.color = color;
}

int RenCache::draw_text(std::shared_ptr<Font> font, const char* text, int x, int y, Color color)
{
    auto rect = Rect
    {
        x,
        y,
        font->get_width(text),
        font->get_height()
    };

    if (Rect::overlap(screen_rect, rect))
    {
        Command& cmd = push_command(DRAW_TEXT);
        cmd.text = text;
        cmd.color = color;
        cmd.font = font;
        cmd.rect = rect;
        cmd.tab_width = font->get_tab_width();
    }

    return x + rect.width;
}

void RenCache::invalidate()
{
    memset(cells_prev, 0xff, sizeof(cells_buf1));
}

void RenCache::begin_frame()
{
    /* reset all cells if the screen width/height has changed */
    const auto size = ren->get_size();
    if (screen_rect.width != size.width || size.height != screen_rect.height)
    {
        screen_rect.width = size.width;
        screen_rect.height = size.height;
        invalidate();
    }

    clip_rect_stack = {Rect::from_size(size)};
    set_clip_rect(clip_rect_stack[0]);
}

void RenCache::update_overlapping_cells(Rect r, unsigned h)
{
    int x1 = r.x / CELL_SIZE;
    int y1 = r.y / CELL_SIZE;
    int x2 = (r.x + r.width) / CELL_SIZE;
    int y2 = (r.y + r.height) / CELL_SIZE;

    for (int y = y1; y <= y2; y++)
    {
        for (int x = x1; x <= x2; x++)
        {
            int idx = cell_idx(x, y);
            Hash::update(&cells[idx], &h, sizeof(h));
        }
    }
}

unsigned Command::compute_hash() const
{
    Hash h;
    h << type;
    h << rect;
    h << color;
    h << font.get();
    h << image.get();
    h << tab_width;
    h.add(text.c_str(), text.length());
    return h.value;
}

void RenCache::end_frame()
{
    std::vector<Rect> rect_buf;
    rect_buf.reserve(CELLS_X * CELLS_Y / 2);

    auto push_rect = [&rect_buf](const Rect& r) {
        // try to merge with existing rectangle
        for (auto& er : rect_buf)
        {
            if (Rect::overlap(er, r))
            {
                er = Rect::merge(er, r);
                return;
            }
        }

        // couldn't merge with previous rectangle: push
        rect_buf.emplace_back(r);
    };

    /* update cells from commands */
    Rect cr = screen_rect;
    for (const auto& cmd : command_buf)
    {
        if (cmd.type == SET_CLIP)
        {
            cr = cmd.rect;
        }
        Rect r = Rect::intersect(cmd.rect, cr);
        if (r.width == 0 || r.height == 0)
        {
            continue;
        }
        const auto h = cmd.compute_hash();
        update_overlapping_cells(r, h);
    }

    /* push rects for all cells changed from last frame, reset cells */
    int max_x = screen_rect.width / CELL_SIZE + 1;
    int max_y = screen_rect.height / CELL_SIZE + 1;
    for (int y = 0; y < max_y; y++)
    {
        for (int x = 0; x < max_x; x++)
        {
            /* compare previous and current cell for change */
            int idx = cell_idx(x, y);
            if (cells[idx] != cells_prev[idx])
            {
                push_rect({x, y, 1, 1});
            }
            cells_prev[idx] = Hash::INITIAL;
        }
    }

    /* expand rects from cells to pixels */
    for (auto& r : rect_buf)
    {
        r.x *= CELL_SIZE;
        r.y *= CELL_SIZE;
        r.width *= CELL_SIZE;
        r.height *= CELL_SIZE;
        r = Rect::intersect(r, screen_rect);
    }

    /* redraw updated regions */
    for (auto& r : rect_buf)
    {
        /* draw */
        ren->set_clip_rect(r);

        for (const auto& cmd : command_buf)
        {
            switch (cmd.type)
            {
            case SET_CLIP:
                ren->set_clip_rect(Rect::intersect(cmd.rect, r));
                break;
            case DRAW_RECT:
                ren->draw_rect(cmd.rect, cmd.color);
                break;
            case DRAW_IMAGE:
                {
                    const auto r = Rect::from_size({cmd.rect.width, cmd.rect.height});
                    ren->draw_image(cmd.image.get(), r, cmd.rect.x, cmd.rect.y, cmd.color);
                }
                break;
            case DRAW_TEXT:
                cmd.font->set_tab_width(cmd.tab_width);
                ren->draw_text(cmd.font.get(), cmd.text.c_str(), cmd.rect.x, cmd.rect.y, cmd.color);
                break;
            }
        }

        if (show_debug)
        {
            const auto color = Color{rng.generate<std::uint8_t>(), rng.generate<std::uint8_t>(),
                                     rng.generate<std::uint8_t>(), 50};
            ren->draw_rect(r, color);
        }
    }

    /* update dirty rects */
    if (rect_buf.empty() == false)
    {
        ren->update_rects(rect_buf);
    }

    /* swap cell buffer and reset */
    std::swap(cells, cells_prev);
    command_buf.clear();
}

RenCache::RenCache(Ren* r)
    : ren(r)
{
}

ClipScope::ClipScope(RenCache* c, const Rect& r)
    : cache(c)
{
    cache->push_clip_rect(r);
}

ClipScope::~ClipScope()
{
    cache->pop_clip_rect();
}
