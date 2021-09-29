#include "libride/document.h"

#include "base/c.h"
#include "base/cc.h"
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

bool operator<=(const position& lhs, const position& rhs)
{
    return lhs.line < rhs.line || (lhs.line == rhs.line && lhs.offset <= rhs.offset);
}

bool operator>=(const position& lhs, const position& rhs)
{
    return lhs.line > rhs.line || (lhs.line == rhs.line && lhs.offset >= rhs.offset);
}


///////////////////////////////////////////////////////////////////////////////////////////////////


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
    return a != b;
}

bool are_overlapping(const sorted_selection& s, const sorted_selection& p)
{
    return 
        s.a <= p.b &&
        p.a <= s.b
        ;
}

bool are_overlapping(const selection& sel, const selection& p)
{
    return are_overlapping(sel.sorted(), p.sorted());
}

///////////////////////////////////////////////////////////////////////////////////////////////////


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

char Document::get_char(const position& pp) const
{
    const auto p = this->sanitize_position(pp);
    return this->lines[Cs(p.line)][Cs(p.offset)];
}

position Document::sanitize_position(const position& p) const
{
    const auto line = keep_within(0, p.line, C(this->lines.size()));
    const auto offset = keep_within(0, p.offset, C(this->lines[Cs(line)].size()));
    return {line, offset};
}

position Document::position_offset(const position& pp, int offset) const
{
    auto p = sanitize_position(pp);
    p.offset = p.offset + offset;
    while( p.line > 0 && p.offset < 0)
    {
        p.line = p.line - 1;
        p.offset = p.offset + C(this->lines[Cs(p.line)].size());
    }
    while(p.line < C(this->lines.size())-1 && p.offset > C(this->lines[Cs(p.line)].size()))
    {
        p.offset = p.offset - C(this->lines[Cs(p.line)].size());
        p.line = p.line + 1;
    }
    return sanitize_position(p);
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

void Document::merge_all_cursors()
{
    auto new_cursors = cursors;

    auto overlaps_any_cursor = [&new_cursors](const selection& ss) -> bool
    {
        const auto s = ss.sorted();
        for(const auto& c: new_cursors)
        {
            if(are_overlapping(c.sorted(), s))
            {
                return true;
            }
        }

        return false;
    };

    for(const auto c: cursors)
    {
        // todo(Gustav): merge, don't just delete one...
        if(overlaps_any_cursor(c) == false)
        {
            new_cursors.emplace_back(c);
        }
    }

    cursors = new_cursors;
}
