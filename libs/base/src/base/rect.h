#pragma once

struct vec2;
struct Size;

struct Rect
{
    // top-left
    int x;
    int y;
    int width;
    int height;

    constexpr Rect()
        : x(0)
        , y(0)
        , width(0)
        , height(0)
    {
    }

    constexpr Rect
    (
        int ax,
        int ay,
        int awidth,
        int aheight
    )
        : x(ax)
        , y(ay)
        , width(awidth)
        , height(aheight)
    {
    }

    Rect(const vec2& p, const Size& s);

    int get_left() const;
    int get_top() const;
    int get_right() const;
    int get_bottom() const;

    static bool overlap(Rect a, Rect b);
    static Rect intersect(Rect a, Rect b);
    static Rect merge(Rect a, Rect b);

    static Rect from_size(const Size& s);

    bool Contains(const vec2& p) const;

    Rect CreateWestFromMaxSize(int max_size) const;
    Rect CreateNorthFromMaxSize(int max_size) const;
    Rect CreateSouthFromMaxSize(int max_size) const;
    Rect CreateEastFromMaxSize(int max_size) const;

    Rect Offset(const vec2& offset) const;

    Rect CreateFromCenterMaxSize(int max_size) const;
    Rect Inset(int inset) const;
};

constexpr const Rect EmptyRect = {0,0, 0,0};
