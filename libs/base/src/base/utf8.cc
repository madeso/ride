#include "base/utf8.h"

std::vector<unsigned> utf8_to_codepoints(const char* text)
{
    std::vector<unsigned> dst;

    const char* p = text;

    unsigned res, n;
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
            res = *p;
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