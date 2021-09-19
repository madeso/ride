#pragma once

#include <string>
#include <sstream>

struct Str
{
    operator std::string() const;

    std::string ToString() const;

    template <typename Type>
    Str& operator<<(const Type& t)
    {
        stream << t;
        return *this;
    }
    
    std::ostringstream stream;
};
