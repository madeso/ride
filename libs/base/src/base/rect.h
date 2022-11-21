#pragma once

#include <cmath>
#include <algorithm>

#include "assert/assert.h"
#include "base/vec2.h"
#include "base/size.h"
#include "base/side.h"
#include "base/c.h"

template<typename T>
struct Rect
{
    using Self = Rect<T>;

    using SizeT = Size<T>;
    using Vec2T = Vec2<T>;

    // bottom-left
    T x;
    T y;
    T width;
    T height;

    constexpr explicit Rect(T same)
        : x(same)
        , y(same)
        , width(same)
        , height(same)
    {
    }

    constexpr Rect
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

    Rect(const Vec2T& p, const SizeT& s)
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
        return y + height;
    }

    T get_right() const
    {
        return x + width;
    }

    T get_bottom() const
    {
        return y;
    }

    void set_left(T new_left)
    {
        const auto old_left = get_left();
        [[maybe_unused]] const auto old_right = get_right();
        const auto diff = old_left - new_left;

        x -= diff;
        width += diff;

        assert(get_left() == new_left);
        assert(get_right() == old_right);
    }

    void set_top(T new_top)
    {
        [[maybe_unused]] const auto old_bottom = get_bottom();
        const auto old_top = get_top();
        const auto diff = old_top - new_top;

        height -= diff;

        assert(get_top() == new_top);
        assert(get_bottom() == old_bottom);
    }

    void set_right(T new_right)
    {
        [[maybe_unused]] const auto old_left = get_left();
        const auto old_right = get_right();
        const auto diff = new_right - old_right;

        width += diff;

        assert(get_right() == new_right);
        assert(get_left() == old_left);
    }

    void set_bottom(T new_bottom)
    {
        const auto old_bottom = get_bottom();
        [[maybe_unused]] const auto old_top = get_top();
        const auto diff = new_bottom - old_bottom;

        height -= diff;
        y += diff;

        assert(get_bottom() == new_bottom);
        assert(get_top() == old_top);
    }

    /*
    static bool overlap(Self a, Self b)
    {
        return b.x + b.width >= a.x
            && b.x <= a.x + a.width
            && b.y + b.height >= a.y
            && b.y <= a.y + a.height
            ;
    }

    static Self intersect(Self a, Self b)
    {
        T x1 = std::max(a.x, b.x);
        T y1 = std::max(a.y, b.y);
        T x2 = std::min(a.x + a.width, b.x + b.width);
        T y2 = std::min(a.y + a.height, b.y + b.height);
        return {x1, y1, std::max(T{0}, x2 - x1), std::max(T{0}, y2 - y1)};
    }

    static Self merge(Self a, Self b)
    {
        T x1 = std::min(a.x, b.x);
        T y1 = std::min(a.y, b.y);
        T x2 = std::max(a.x + a.width, b.x + b.width);
        T y2 = std::max(a.y + a.height, b.y + b.height);
        return {x1, y1, x2 - x1, y2 - y1};
    }
    */

    bool contains(const Vec2T& p) const
    {
        const auto cx = get_left() <= p.x && p.x <= get_right();
        const auto cy = get_bottom() <= p.y && p.y <= get_top();
        return cx && cy;
    }

    static Self from_size(const SizeT& s)
    {
        return {T{0}, T{0}, s.width, s.height};
    }


    static Self from_ltrb(T left, T top, T right, T bottom)
    {
        XASSERT(left <= right && top >= bottom, "{} <= {} && {} >= {}"_format(left, right, top, bottom));
        return {left, bottom, right - left, top-bottom};
    }

    static Self from_lrtb(T left, T right, T top, T bottom)
    {
        XASSERT(left <= right && top >= bottom, "{} <= {} && {} >= {}"_format(left, right, top, bottom));
        return {left, bottom, right - left, top-bottom};
    }

    Self cut_left(T a)
    {
        T ox = this->get_left();
        this->set_left(std::min(this->get_right(), this->get_left() + a));
        return from_ltrb(ox, this->get_top(), this->get_left(), this->get_bottom());
    }

    Self cut_right(T a)
    {
        T right = this->get_right();
        this->set_right(std::max(this->x, this->get_right() - a));
        return from_ltrb(this->get_right(), this->get_top(), right, this->get_bottom());
    }

    Self cut_top(T a)
    {
        T oy = this->get_top();
        this->set_top(std::max(this->get_bottom(), this->get_top() - a));
        return from_ltrb(this->get_left(), oy, this->get_right(), this->get_top());
    }

    Self cut_bottom(T a)
    {
        T bottom = this->get_bottom();
        const auto top = this->get_top();
        const auto new_bottom = this->get_bottom() + a;
        this->set_bottom(std::min(top, new_bottom));
        return from_ltrb(this->get_left(), this->get_bottom(), this->get_right(), bottom);
    }

    Self get_cut_left(T a) const
    {
        auto r = *this;
        return r.cut_left(a);
    }

    Self get_cut_right(T a) const
    {
        auto r = *this;
        return r.cut_right(a);
    }

    Self get_cut_top(T a) const
    {
        auto r = *this;
        return r.cut_top(a);
    }

    Self get_cut_bottom(T a) const
    {
        auto r = *this;
        return r.cut_bottom(a);
    }


    Self cut(Side side, T a)
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

    Self get_cut(Side side, T a) const
    {
        auto r = *this;
        return r.cut(side, a);
    }


    Self get_offset(const Vec2T& offset) const
    {
        const auto p = Vec2T{x, y} + offset;
        return {p.x, p.y, width, height};
    }

    /*
    Self CreateFromCenterMaxSize(T max_size) const
    {
        const auto s = std::min(max_size, width);
        const auto h = static_cast<T>(static_cast<float>(width - s) / 2.0f);
        return {x + h, y, s, width};
    }
    */

    Self get_inset(T inset) const
    {
        const auto inset2 = inset + inset;
        // if(inset2 > width || inset2 > height) { return {x, y, 0, 0}; }
        return {x+inset, y+inset, width - inset2, height - inset2};
    }

};

using Recti = Rect<int>;
using Rectf = Rect<float>;

constexpr Rectf Cint_to_float(const Recti r)
{
    return
    {
        Cint_to_float(r.x),
        Cint_to_float(r.y),
        Cint_to_float(r.width),
        Cint_to_float(r.height)
    };
}

constexpr const Recti EmptyRect = {0,0, 0,0};


template <typename T>
struct fmt::formatter<Rect<T>> : formatter<string_view>
{
    template <typename FormatContext>
    auto format(const Rect<T>& r, FormatContext& ctx)
    {
        return format_to(ctx.out(), "[l/r {} {} / tb {} {}]", r.get_left(), r.get_right(), r.get_top(), r.get_bottom());
    }
};
