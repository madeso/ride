#include "base/rect.h"

#include <cmath>
#include <algorithm>
#include <cassert>

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

void Rect::set_left(int new_left)
{
    const auto old = get_left();
    const auto diff = old - new_left;

    x -= diff;
    width += diff;

    assert(get_left() == new_left);
}

void Rect::set_top(int new_top)
{
    const auto old = get_top();
    const auto diff = old - new_top;

    y -= diff;
    height += diff;

    assert(get_top() == new_top);
}

void Rect::set_right(int new_right)
{
    const auto old = get_right();
    const auto diff = new_right - old;

    width += diff;

    assert(get_right() == new_right);
}

void Rect::set_bottom(int new_bottom)
{
    const auto old = get_bottom();
    const auto diff = new_bottom - old;

    height += diff;

    assert(get_bottom() == new_bottom);
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

Rect Rect::FromL(int left, int top, int right, int bottom)
{
    return {left, top, right - left, bottom - top};
}

Rect Rect::cut_left(int a)
{
    int x = this->x;
    this->set_left(std::min(this->get_right(), this->x + a));
    return FromL(x, this->y, this->x, this->get_bottom());
}

Rect Rect::cut_right(int a)
{
    int right = this->get_right();
    this->set_right(std::max(this->x, this->get_right() - a));
    return FromL(this->get_right(), this->y, right, this->get_bottom());
}

Rect Rect::cut_top(int a)
{
    int y = this->y;
    this->set_top(std::min(this->get_bottom(), this->y + a));
    return FromL(this->x, y, this->get_right(), this->y);
}

Rect Rect::cut_bottom(int a)
{
    int bottom = this->get_bottom();
    this->set_bottom(std::max(this->y, this->get_bottom() - a));
    return FromL(this->x, this->get_bottom(), this->get_right(), bottom);
}

Rect Rect::get_cut_left(int a) const
{
    auto r = *this;
    return r.cut_left(a);
}

Rect Rect::get_cut_right(int a) const
{
    auto r = *this;
    return r.cut_right(a);
}

Rect Rect::get_cut_top(int a) const
{
    auto r = *this;
    return r.cut_top(a);
}

Rect Rect::get_cut_bottom(int a) const
{
    auto r = *this;
    return r.cut_bottom(a);
}


Rect Rect::cut(Side side, int a)
{
    switch(side)
    {
        case Side::left:   return cut_left(   a);
        case Side::right:  return cut_right(  a);
        case Side::top:    return cut_top(    a);
        case Side::bottom: return cut_bottom( a);
        default:
            assert(false && "invalid side");
            return {};
    }
}

Rect Rect::get_cut(Side side, int a) const
{
    auto r = *this;
    return r.cut(side, a);
}

/*

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

*/

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
    return {x+inset, y+inset, width - inset*2, height - inset*2};
}
