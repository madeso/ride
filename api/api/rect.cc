#include "api/rect.h"

#include <cmath>

#include "api/vec2.h"
#include "api/size.h"

Rect::Rect()
{
}

Rect::Rect(
    int ax,
    int ay,
    int awidth,
    int aheight)
    : x(ax)
    , y(ay)
    , width(awidth)
    , height(aheight)
{
}

Rect::Rect(const vec2& p, const Size& s)
    : x(p.x)
    , y(p.y)
    , width(s.width)
    , height(s.height)
{
}

int Rect::get_left() const
{
    return x;
}

int Rect::get_top() const
{
    return y;
}

int Rect::get_right() const
{
    return x + width;
}

int Rect::get_bottom() const
{
    return y + height;
}

bool Rect::overlap(Rect a, Rect b)
{
    return b.x + b.width >= a.x && b.x <= a.x + a.width && b.y + b.height >= a.y &&
           b.y <= a.y + a.height;
}

Rect Rect::intersect(Rect a, Rect b)
{
    int x1 = std::max(a.x, b.x);
    int y1 = std::max(a.y, b.y);
    int x2 = std::min(a.x + a.width, b.x + b.width);
    int y2 = std::min(a.y + a.height, b.y + b.height);
    return {x1, y1, std::max(0, x2 - x1), std::max(0, y2 - y1)};
}

Rect Rect::merge(Rect a, Rect b)
{
    int x1 = std::min(a.x, b.x);
    int y1 = std::min(a.y, b.y);
    int x2 = std::max(a.x + a.width, b.x + b.width);
    int y2 = std::max(a.y + a.height, b.y + b.height);
    return {x1, y1, x2 - x1, y2 - y1};
}

Rect Rect::from_size(const Size& s)
{
    return {0, 0, s.width, s.height};
}

