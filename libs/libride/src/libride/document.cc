#include "libride/document.h"

#include "base/c.h"
#include "base/filesystem.h"

namespace
{
    bool is_same(const position& lhs, const position& rhs)
    {
        return lhs.line == rhs.line
            && lhs.offset == rhs.offset;
    }
}

bool operator==(const position& lhs, const position& rhs)
{
    return is_same(lhs, rhs) == true;
}

bool operator!=(const position& lhs, const position& rhs)
{
    return is_same(lhs, rhs) == false;
}

bool operator<(const position& lhs, const position& rhs)
{
    return lhs.line < rhs.line || (lhs.line == rhs.line && lhs.offset < rhs.offset);
}

bool operator>(const position& lhs, const position& rhs)
{
    return lhs.line > rhs.line || (lhs.line == rhs.line && lhs.offset > rhs.offset);
}

sorted_selection selection::sorted() const
{
    if( a > b )
    {
        return {b, a};
    }
    return {a, b};
}

bool selection::is_selection() const
{
    return a == b;
}


bool Document::LoadFile(filesystem* fs, const std::string& path)
{
    lines.clear();
    
    const auto result = fs->load_file
    (
        path,
        [this](const std::string& line)
        {
            lines.emplace_back(line);
        }
    );

    return result;
}


int Document::GetNumberOfLines() const
{
    return C(lines.size());
}

std::string Document::GetLineAt(int y) const
{
    if(y < 0)
    {
        return "";
    }
    else if(y >= C(lines.size()))
    {
        return "";
    }
    else
    {
        return lines[Cs(y)];
    }
}
