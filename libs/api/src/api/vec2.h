#pragma once

struct vec2
{
    int x;
    int y;
};

vec2 operator+(const vec2& lhs, const vec2& rhs);
vec2 operator-(const vec2& lhs, const vec2& rhs);
