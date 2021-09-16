#ifndef RIDE_STR_H
#define RIDE_STR_H

#include <string>
#include <sstream>

namespace ride
{
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
}

#endif  // RIDE_STR_H
