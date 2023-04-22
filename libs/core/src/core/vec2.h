#pragma once


namespace ride
{


template<typename T>
struct Vec2
{
    T x;
    T y;
};

#define TPL template<typename T>
#define VEC Vec2<T>

TPL VEC operator+(const VEC& lhs, const VEC& rhs) { return {lhs.x + rhs.x, lhs.y + rhs.y}; }
TPL VEC operator-(const VEC& lhs, const VEC& rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y}; }

#undef VEC
#undef TPL

using vec2i = Vec2<int>;

template<typename S, typename T> S& operator<<(S& s, const Vec2<T>& v)
{
    return s
        << "("
        << v.x
        << ", "
        << v.y
        << ")"
        ;
}


}

