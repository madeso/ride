#pragma once

namespace api
{
    template<typename T>
    T abs(T t)
    {
        if( t >= 0)
        {
            return t;
        }
        else
        {
            return -t;
        }
    }
}
