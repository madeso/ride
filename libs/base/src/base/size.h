#pragma once

template<typename T>
struct size
{
    T width;
    T height;

    template<typename Y>
    size<Y> as() const
    {
        return size<Y> { Y{width}, Y{height} };
    }
};

using sizei = size<int>;
