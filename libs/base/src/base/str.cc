#include "base/str.h"


Str::operator std::string() const
{
    return ToString();
}


std::string Str::ToString() const
{
    return stream.str();
}
