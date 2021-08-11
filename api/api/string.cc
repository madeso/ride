#include "api/string.h"

#include <algorithm>

std::size_t replace_all(std::string& inout, std::string_view what, std::string_view with)
{
    std::size_t count{};
    for (std::string::size_type pos{};
         inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
}

char to_lower_char(char c)
{
    if('A' <= c && c <= 'Z')
    {
        return c - 'A' + 'a';
    }
    else
    {
        return c;
    }
}

std::string to_lower(const std::string& str)
{
    std::string s = str;
    std::transform(s.begin(), s.end(), s.begin(), to_lower_char);
    return s;
}
