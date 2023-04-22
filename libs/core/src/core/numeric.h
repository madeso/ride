#pragma once

namespace ride
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

