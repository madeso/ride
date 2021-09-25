#include "api/rencache.h"

#include <cassert>

#include "base/hash.h"

/* a cache over the software renderer -- all drawing operations are stored as
** commands when issued. At the end of the frame we write the commands to a grid
** of hash values, take the cells that have changed since the previous frame,
** merge them into dirty rectangles and redraw only those regions */

rect<int> C(const rect<dip>& r)
{
    return
    {
        r.x.value,
        r.y.value,
        r.width.value,
        r.height.value
    };
}

rect<dip> C(const rect<int>& r)
{
    return
    {
        dip{r.x},
        dip{r.y},
        dip{r.width},
        dip{r.height}
    };
}


int cell_idx(int x, int y)
{
    return x + y * CELLS_X;
}

void RenCache::push_clip_rect(const rect<dip>& rr)
{
    const auto top = *clip_rect_stack.rbegin();
    auto rect = rr;
    auto r = rect.x + rect.width;
    auto b = rect.y + rect.height;
    const auto r2 = top.x + top.width;
    const auto b2 = top.y + top.height;
    rect.x = std::max(rect.x, top.x);
    rect.y = std::max(rect.y, top.y);
    b = std::min(b, b2);
    r = std::min(r, r2);
    rect.width = r - rect.x;
    rect.height = b - rect.y;
    clip_rect_stack.push_back(rect);
    set_clip_rect(rect);
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

void RenCache::set_clip_rect(const rect<dip>& rect)
{
    Command& cmd = push_command(SET_CLIP);
    cmd.rect_arg = ::rect<dip>::intersect(rect, screen_rect);
}

void RenCache::draw_rect(rect<dip> rect, Color color)
{
    if (!::rect<dip>::overlap(screen_rect, rect))
    {
        return;
    }
    Command& cmd = push_command(DRAW_RECT);
    cmd.rect_arg = rect;
    cmd.color = color;
}

void RenCache::draw_image(std::shared_ptr<Image> image, dip x, dip y, Color color)
{
    auto rect = ::rect<dip>
    {
        x,
        y,
        dip{image->width},
        dip{image->height}
    };

    if (!::rect<dip>::overlap(screen_rect, rect))
    {
        return;
    }
    Command& cmd = push_command(DRAW_IMAGE);
    cmd.rect_arg = rect;
    cmd.image = image;
    cmd.color = color;
}

dip RenCache::draw_text(std::shared_ptr<Font> font, const std::string& text, dip x, dip y, Color color)
{
    auto rect = ::rect<dip>
    {
        x,
        y,
        font->get_width(text),
        font->get_height()
    };

    if (::rect<dip>::overlap(screen_rect, rect))
    {
        Command& cmd = push_command(DRAW_TEXT);
        cmd.text = text;
        cmd.color = color;
        cmd.font = font;
        cmd.rect_arg = rect;
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
    const auto size = ren->get_size().as<dip>();
    if (screen_rect.width != size.width || size.height != screen_rect.height)
    {
        screen_rect.width = size.width;
        screen_rect.height = size.height;
        invalidate();
    }

    clip_rect_stack = {rect<dip>::from_size(size)};
    set_clip_rect(clip_rect_stack[0]);
}

void RenCache::update_overlapping_cells(const rect<dip>& r, unsigned h)
{
    int x1 = r.x.value / CELL_SIZE;
    int y1 = r.y.value / CELL_SIZE;
    int x2 = (r.x + r.width).value / CELL_SIZE;
    int y2 = (r.y + r.height).value / CELL_SIZE;

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
    h << rect_arg;
    h << color;
    h << font.get();
    h << image.get();
    h << tab_width;
    h.add(text.c_str(), text.length());
    return h.value;
}

void RenCache::end_frame()
{
    std::vector<recti> rect_buf;
    rect_buf.reserve(CELLS_X * CELLS_Y / 2);

    auto push_rect = [&rect_buf](const recti& r) {
        // try to merge with existing rectangle
        for (auto& er : rect_buf)
        {
            if (recti::overlap(er, r))
            {
                er = recti::merge(er, r);
                return;
            }
        }

        // couldn't merge with previous rectangle: push
        rect_buf.emplace_back(r);
    };

    /* update cells from commands */
    rect<dip> cr = screen_rect;
    for (const auto& cmd : command_buf)
    {
        if (cmd.type == SET_CLIP)
        {
            cr = cmd.rect_arg;
        }
        rect<dip> r = rect<dip>::intersect(cmd.rect_arg, cr);
        if (r.width.value == 0 || r.height.value == 0)
        {
            continue;
        }
        const auto h = cmd.compute_hash();
        update_overlapping_cells(r, h);
    }

    /* push rects for all cells changed from last frame, reset cells */
    int max_x = screen_rect.width.value / CELL_SIZE + 1;
    int max_y = screen_rect.height.value / CELL_SIZE + 1;
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
        r = recti::intersect(r, C(screen_rect));
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
                ren->set_clip_rect(recti::intersect(C(cmd.rect_arg), r));
                break;
            case DRAW_RECT:
                ren->draw_rect(C(cmd.rect_arg), cmd.color);
                break;
            case DRAW_IMAGE:
                {
                    const auto rr = rect<dip>::from_size({cmd.rect_arg.width, cmd.rect_arg.height});
                    ren->draw_image(cmd.image.get(), C(rr), cmd.rect_arg.x.value, cmd.rect_arg.y.value, cmd.color);
                }
                break;
            case DRAW_TEXT:
                cmd.font->set_tab_width(cmd.tab_width);
                ren->draw_text(cmd.font.get(), cmd.text.c_str(), cmd.rect_arg.x.value, cmd.rect_arg.y.value, cmd.color);
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

ClipScope::ClipScope(RenCache* c, const rect<dip>& r)
    : cache(c)
{
    cache->push_clip_rect(r);
}

ClipScope::~ClipScope()
{
    cache->pop_clip_rect();
}
