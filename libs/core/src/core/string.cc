#include "core/string.h"

#include <algorithm>
#include <sstream>

std::size_t replace_all_inplace(std::string& inout, std::string_view what, std::string_view with)
{
    std::size_t count{};
    for (std::string::size_type pos{};
         inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
}

std::string replace_all_f(const std::string& inout, std::string_view what, std::string_view with)
{
    std::string inplace = inout;
    replace_all_inplace(inplace, what, with);
    return inplace;
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

std::string trim_right(const std::string& string_to_trim, const std::string& trim_characters)
{
    return std::string(string_to_trim).erase(string_to_trim.find_last_not_of(trim_characters) + 1);
}

std::string trim_left(const std::string& string_to_trim, const std::string& trim_characters)
{
    return std::string(string_to_trim).erase(0, string_to_trim.find_first_not_of(trim_characters));
}

std::string trim(const std::string& string_to_trim, const std::string& trim_characters)
{
    return trim_right(trim_left(string_to_trim, trim_characters), trim_characters);
}

template <typename Out>
void split_on_delim(const std::string& s, char delim, Out result)
{
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim))
    {
        *(result++) = item;
    }
}

std::vector<std::string> split_on_delim(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    split_on_delim(s, delim, std::back_inserter(elems));
    return elems;
}


constexpr const char* const WHITESPACE = " \t\r\n";

bool is_only_whitepace(const std::string& str)
{
    return str.find_first_not_of(WHITESPACE) == std::string::npos;
}

int index_of_non_whitespace(const std::string& str)
{
    const auto index = str.find_first_not_of(WHITESPACE);
    return index == std::string::npos ? 0 : static_cast<int>(index);
}

std::vector<std::string> supersplit(const std::string& str, char delim)
{
    std::vector<std::string> ret;
    std::string buffer;

    for(auto c: str)
    {
        if(c == delim)
        {
            ret.emplace_back(buffer);
            buffer = "";
        }
        else
        {
            buffer += c;
        }
    }

    ret.emplace_back(buffer);

    return ret;
}

std::vector<std::string> split_string_by_newline(const std::string& str)
{
    return supersplit(str, '\n');
}
