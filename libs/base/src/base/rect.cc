#include "base/rect.h"

#include <cmath>
#include <algorithm>

#include "base/vec2.h"
#include "base/size.h"

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

bool Rect::Contains(const vec2& p) const
{
    const auto cx = p.x >= x && x + width >= p.x;
    const auto cy = p.y >= y && y + width >= p.y;
    return cx && cy;
}


Rect Rect::CreateWestFromMaxSize(int max_size) const
{
    return {x, y, std::min(max_size, width), width};
}


Rect Rect::CreateNorthFromMaxSize(int max_size) const
{
    return {x, y, width, std::min(max_size, width)};
}

Rect Rect::CreateSouthFromMaxSize(int max_size) const
{
    const auto s = std::min(max_size, width);
    return {x, y + width - s, width, s};
}

Rect Rect::CreateEastFromMaxSize(int max_size) const
{
    const auto s = std::min(max_size, width);
    return {x + width - s, y, s, width};
}

Rect Rect::Offset(const vec2& offset) const
{
    const auto p = vec2{x, y} + offset;
    return {p.x, p.y, width, height};
}

Rect Rect::CreateFromCenterMaxSize(int max_size) const
{
    const auto s = std::min(max_size, width);
    const auto h = static_cast<int>(static_cast<float>(width - s) / 2.0f);
    return {x + h, y, s, width};
}

Rect Rect::Inset(int inset) const
{
    // todo(Gustav): handle when inset is greater than size
    return {x+inset, y+inset, width - inset*2, width - inset*2};
}
