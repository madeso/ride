#pragma once

#include <cmath>
#include <algorithm>
#include <cassert>

#include "base/vec2.h"
#include "base/size.h"
#include "base/side.h"

template<typename T>
struct rect
{
    using self = rect<T>;

    using size_type = size<T>;
    using vec2_type = vec2<T>;

    // top-left
    T x;
    T y;
    T width;
    T height;

    constexpr explicit rect(T same)
        : x(same)
        , y(same)
        , width(same)
        , height(same)
    {
    }

    constexpr rect
    (
        T ax,
        T ay,
        T awidth,
        T aheight
    )
        : x(ax)
        , y(ay)
        , width(awidth)
        , height(aheight)
    {
    }

    rect(const vec2_type& p, const size_type& s)
        : x(p.x)
        , y(p.y)
        , width(s.width)
        , height(s.height)
    {
    }

    T get_left() const
    {
        return x;
    }

    T get_top() const
    {
        return y;
    }

    T get_right() const
    {
        return x + width;
    }

    T get_bottom() const
    {
        return y + height;
    }

    void set_left(T new_left)
    {
        const auto old = get_left();
        const auto diff = old - new_left;

        x -= diff;
        width += diff;

        assert(get_left() == new_left);
    }

    void set_top(T new_top)
    {
        const auto old = get_top();
        const auto diff = old - new_top;

        y -= diff;
        height += diff;

        assert(get_top() == new_top);
    }

    void set_right(T new_right)
    {
        const auto old = get_right();
        const auto diff = new_right - old;

        width += diff;

        assert(get_right() == new_right);
    }

    void set_bottom(T new_bottom)
    {
        const auto old = get_bottom();
        const auto diff = new_bottom - old;

        height += diff;

        assert(get_bottom() == new_bottom);
    }

    static bool overlap(self a, self b)
    {
        return b.x + b.width >= a.x && b.x <= a.x + a.width && b.y + b.height >= a.y &&
            b.y <= a.y + a.height;
    }

    static self intersect(self a, self b)
    {
        T x1 = std::max(a.x, b.x);
        T y1 = std::max(a.y, b.y);
        T x2 = std::min(a.x + a.width, b.x + b.width);
        T y2 = std::min(a.y + a.height, b.y + b.height);
        return {x1, y1, std::max(T{0}, x2 - x1), std::max(T{0}, y2 - y1)};
    }

    static self merge(self a, self b)
    {
        T x1 = std::min(a.x, b.x);
        T y1 = std::min(a.y, b.y);
        T x2 = std::max(a.x + a.width, b.x + b.width);
        T y2 = std::max(a.y + a.height, b.y + b.height);
        return {x1, y1, x2 - x1, y2 - y1};
    }

    static self from_size(const size_type& s)
    {
        return {T{0}, T{0}, s.width, s.height};
    }

    bool contains(const vec2_type& p) const
    {
        const auto cx = p.x >= x && x + width >= p.x;
        const auto cy = p.y >= y && y + height >= p.y;
        return cx && cy;
    }

    static self FromL(T left, T top, T right, T bottom)
    {
        return {left, top, right - left, bottom - top};
    }

    self cut_left(T a)
    {
        T ox = this->x;
        this->set_left(std::min(this->get_right(), this->x + a));
        return FromL(ox, this->y, this->x, this->get_bottom());
    }

    self cut_right(T a)
    {
        T right = this->get_right();
        this->set_right(std::max(this->x, this->get_right() - a));
        return FromL(this->get_right(), this->y, right, this->get_bottom());
    }

    self cut_top(T a)
    {
        T oy = this->y;
        this->set_top(std::min(this->get_bottom(), this->y + a));
        return FromL(this->x, oy, this->get_right(), this->y);
    }

    self cut_bottom(T a)
    {
        T bottom = this->get_bottom();
        this->set_bottom(std::max(this->y, this->get_bottom() - a));
        return FromL(this->x, this->get_bottom(), this->get_right(), bottom);
    }

    self get_cut_left(T a) const
    {
        auto r = *this;
        return r.cut_left(a);
    }

    self get_cut_right(T a) const
    {
        auto r = *this;
        return r.cut_right(a);
    }

    self get_cut_top(T a) const
    {
        auto r = *this;
        return r.cut_top(a);
    }

    self get_cut_bottom(T a) const
    {
        auto r = *this;
        return r.cut_bottom(a);
    }


    self cut(Side side, T a)
    {
        switch(side)
        {
            case Side::left:   return cut_left(   a);
            case Side::right:  return cut_right(  a);
            case Side::top:    return cut_top(    a);
            case Side::bottom: return cut_bottom( a);
            default:
                assert(false && "invalid side");
                return cut_left(a);
        }
    }

    self get_cut(Side side, T a) const
    {
        auto r = *this;
        return r.cut(side, a);
    }

    self Offset(const vec2_type& offset) const
    {
        const auto p = vec2_type{x, y} + offset;
        return {p.x, p.y, width, height};
    }

    self CreateFromCenterMaxSize(T max_size) const
    {
        const auto s = std::min(max_size, width);
        const auto h = static_cast<T>(static_cast<float>(width - s) / 2.0f);
        return {x + h, y, s, width};
    }

    self Inset(T inset) const
    {
        // todo(Gustav): handle when inset is greater than size
        const auto inset2 = inset + inset;
        return {x+inset, y+inset, width - inset2, height - inset2};
    }

};

using recti = rect<int>;

constexpr const recti EmptyRect = {0,0, 0,0};
