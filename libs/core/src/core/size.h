#pragma once

template<typename T>
struct Size
{
    T width;
    T height;

    template<typename Y>
    Size<Y> as() const
    {
        return Size<Y> { Y{width}, Y{height} };
    }
};

using Sizei = Size<int>;
