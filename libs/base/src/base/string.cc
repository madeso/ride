#include "base/string.h"

#include <algorithm>
#include <sstream>

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

std::string TrimRight(const std::string& string_to_trim, const std::string& trim_characters)
{
    return std::string(string_to_trim).erase(string_to_trim.find_last_not_of(trim_characters) + 1);
}

std::string TrimLeft(const std::string& string_to_trim, const std::string& trim_characters)
{
    return std::string(string_to_trim).erase(0, string_to_trim.find_first_not_of(trim_characters));
}

std::string Trim(const std::string& string_to_trim, const std::string& trim_characters)
{
    return TrimRight(TrimLeft(string_to_trim, trim_characters), trim_characters);
}

template <typename Out>
void Split(const std::string& s, char delim, Out result)
{
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim))
    {
        *(result++) = item;
    }
}

std::vector<std::string> Split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    Split(s, delim, std::back_inserter(elems));
    return elems;
}