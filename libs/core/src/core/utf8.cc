#include "core/utf8.h"

#include <cassert>

namespace
{
    constexpr u32 c_char_to_unsigned(char c)
    {
        return static_cast<unsigned>(c);
    }
    static_assert(c_char_to_unsigned(' ') == 32, "c_char_to_unsigned doesn't work");

    constexpr u8 char_to_u8(char c)
    {
        return static_cast<u8>(c);
    }
    static_assert(char_to_u8(' ') == 32, "char_to_u8 functions doesn't work");

    constexpr bool is_in_range(u8 lower, u8 cc, u8 upper)
    {
        return lower <= cc && cc <= upper;
    }

    constexpr bool is_utf8_char(u8 c, bool first)
    {
        if(first) return is_in_range(0x00, c, 0x7f) || is_in_range(0xc2, c, 0xf4);
        else return is_in_range(0x80, c, 0xbf);
    }
}

bool is_utf8_cont(u8 byte)
{
    return byte >= 0x80 && byte < 0xc0;
}

bool is_utf8_cont_char(char c)
{
    return is_utf8_cont(char_to_u8(c));
}

std::vector<u32> utf8_to_codepoints(const std::string& text)
{
    std::vector<u32> dst;

    const char* p = text.c_str();

    u32 res;
    u32 n;
    while (*p)
    {
        switch (*p & 0xf0)
        {
        case 0xf0:
            res = *p & 0x07;
            n = 3;
            break;
        case 0xe0:
            res = *p & 0x0f;
            n = 2;
            break;
        case 0xd0:
        case 0xc0:
            res = *p & 0x1f;
            n = 1;
            break;
        default:
            res = c_char_to_unsigned(*p);
            n = 0;
            break;
        }
        while (n--)
        {
            res = (res << 6) | (*(++p) & 0x3f);
        }
        dst.emplace_back(res);
        p = p + 1;
    }

    return dst;
}

std::vector<std::string> utf8_chars(const std::string& text)
{
    std::vector<std::string> r;
    std::string buffer;
    auto clear_buffer = [&]()
    {
        if(buffer.empty() == false)
        {
            r.emplace_back(buffer);
            buffer = "";
        }
    };
    for(char c: text)
    {
        if(is_utf8_char(char_to_u8(c), true))
        {
            clear_buffer();
            buffer += c;
        }
        else if(is_utf8_char(char_to_u8(c), false))
        {
            assert(is_utf8_cont(char_to_u8(c)));
            buffer += c;
        }
        else
        {
            // invalid utf8 character?
            clear_buffer();
        }
    }
    clear_buffer();
    return r;
}
