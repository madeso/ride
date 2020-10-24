#include "ride/str.h"


namespace ride
{
    Str::operator std::string() const
    {
        return ToString();
    }


    std::string Str::ToString() const
    {
        return stream.str();
    }
}
