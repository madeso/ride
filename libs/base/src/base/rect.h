#pragma once

struct vec2;
struct Size;

enum class Side
{
    left,
    right,
    top,
    bottom
};

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

    void set_left(int new_left);
    void set_top(int new_top);
    void set_right(int new_right);
    void set_bottom(int new_bottom);

    static bool overlap(Rect a, Rect b);
    static Rect intersect(Rect a, Rect b);
    static Rect merge(Rect a, Rect b);

    static Rect from_size(const Size& s);

    bool Contains(const vec2& p) const;

    static Rect FromL(int left, int top, int right, int bottom);

    Rect cut_left(int a);
    Rect cut_right(int a);
    Rect cut_top(int a);
    Rect cut_bottom(int a);

    Rect get_cut_left(int a) const;
    Rect get_cut_right(int a) const;
    Rect get_cut_top(int a) const;
    Rect get_cut_bottom(int a) const;

    Rect cut(Side side, int a);
    Rect get_cut(Side side, int a) const;

    Rect Offset(const vec2& offset) const;

    Rect CreateFromCenterMaxSize(int max_size) const;
    Rect Inset(int inset) const;
};

constexpr const Rect EmptyRect = {0,0, 0,0};
