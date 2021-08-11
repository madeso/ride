#pragma once

struct vec2;
struct Size;

struct Rect
{
    int x;
    int y;
    int width;
    int height;

    Rect();

    Rect(
    int ax,
    int ay,
    int awidth,
    int aheight);

    Rect(const vec2& p, const Size& s);

    int get_left() const;
    int get_top() const;
    int get_right() const;
    int get_bottom() const;

    static bool overlap(Rect a, Rect b);
    static Rect intersect(Rect a, Rect b);
    static Rect merge(Rect a, Rect b);

    static Rect from_size(const Size& s);
};
