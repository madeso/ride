#pragma once

template<typename T>
struct vec2
{
    T x;
    T y;
};

#define TPL template<typename T>
#define VEC vec2<T>

TPL VEC operator+(const VEC& lhs, const VEC& rhs) { return {lhs.x + rhs.x, lhs.y + rhs.y}; }
TPL VEC operator-(const VEC& lhs, const VEC& rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y}; }

#undef VEC
#undef TPL

using vec2i = vec2<int>;
