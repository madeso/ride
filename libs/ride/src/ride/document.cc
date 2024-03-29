#include "ride/document.h"

#include <iostream>

#include "core/c.h"
#include "core/cc.h"
#include "core/filesystem.h"
#include "core/string.h"

#include "ride/document.commands.h"


namespace ride::libride
{


namespace
{
    bool is_same(const Position& lhs, const Position& rhs)
    {
        return lhs.line == rhs.line
            && lhs.offset == rhs.offset;
    }
}

bool operator==(const Position& lhs, const Position& rhs)
{
    return is_same(lhs, rhs) == true;
}

bool operator!=(const Position& lhs, const Position& rhs)
{
    return is_same(lhs, rhs) == false;
}

bool operator<(const Position& lhs, const Position& rhs)
{
    return lhs.line < rhs.line || (lhs.line == rhs.line && lhs.offset < rhs.offset);
}

bool operator>(const Position& lhs, const Position& rhs)
{
    return lhs.line > rhs.line || (lhs.line == rhs.line && lhs.offset > rhs.offset);
}

bool operator<=(const Position& lhs, const Position& rhs)
{
    return lhs.line < rhs.line || (lhs.line == rhs.line && lhs.offset <= rhs.offset);
}

bool operator>=(const Position& lhs, const Position& rhs)
{
    return lhs.line > rhs.line || (lhs.line == rhs.line && lhs.offset >= rhs.offset);
}

std::ostream& operator<<(std::ostream& s, const Position& p)
{
    s << "(" << p.line << ", ";
    if(p.offset == Position::max_offset)
    {
        s << "<max>";
    }
    else
    {
        s << p.offset;
    }
    s << ")";
    
    return s;
}

namespace
{
    bool is_same(const Selection& lhs, const Selection& rhs)
    {
        return lhs.a == rhs.a
            && lhs.b == rhs.b
        ;
    }
}

bool operator==(const Selection& lhs, const Selection& rhs)
{
    return is_same(lhs, rhs) == true;
}


bool operator!=(const Selection& lhs, const Selection& rhs)
{
    return is_same(lhs, rhs) == false;
}



///////////////////////////////////////////////////////////////////////////////////////////////////


SortedSelection Selection::as_sorted() const
{
    if( a > b )
    {
        return {b, a};
    }
    return {a, b};
}

bool Selection::is_selection() const
{
    return a != b;
}

bool are_overlapping(const SortedSelection& s, const SortedSelection& p)
{
    return 
        s.a <= p.b &&
        p.a <= s.b
        ;
}

bool are_overlapping(const Selection& sel, const Selection& p)
{
    return are_overlapping(sel.as_sorted(), p.as_sorted());
}

///////////////////////////////////////////////////////////////////////////////////////////////////


bool Document::load_file(FileSystem* fs, const std::string& path)
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

char Document::get_char(const Position& pp) const
{
    const auto p = this->sanitize_position(pp);
    const auto line = Cs(p.line);
    const auto offset = Cs(p.offset);
    if(line >= lines.size()) { return '\n'; }
    const auto text = this->lines[line];
    if(offset >= text.size()) { return '\n'; }
    return text[offset];
}

int Document::sanitize_line(int new_line) const
{
    return keep_within(0, new_line, C(this->lines.size()));
}

Position Document::sanitize_position(const Position& p) const
{
    const auto line = sanitize_line(p.line);
    const auto offset = keep_within(0, p.offset, C(this->lines[Cs(line)].size()));
    return {line, offset};
}

Position Document::position_offset(const Position& pp, int offset) const
{
    auto p = sanitize_position(pp);
    p.offset = p.offset + offset;
    while( p.line > 0 && p.offset < 0)
    {
        p.line = p.line - 1;
        p.offset = p.offset + C(this->lines[Cs(p.line)].size()+1);
    }
    while(p.line < C(this->lines.size())-1 && p.offset > C(this->lines[Cs(p.line)].size()))
    {
        p.offset = p.offset - C(this->lines[Cs(p.line)].size()+1);
        p.line = p.line + 1;
    }
    return sanitize_position(p);
}

int Document::get_number_of_lines() const
{
    return C(lines.size());
}

std::string Document::get_line_at(int y) const
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

void VirtualView::merge_all_cursors()
{
    auto new_cursors = std::vector<Selection>{};

    auto get_index_of_first_overlapping_cursor = [&new_cursors](const Selection& ss) -> std::optional<std::size_t>
    {
        std::size_t index = 0;
        const auto s = ss.as_sorted();
        for(const auto& c: new_cursors)
        {
            if(are_overlapping(c.as_sorted(), s))
            {
                return index;
            }

            index += 1;
        }

        return {};
    };

    for(const auto c: cursors)
    {
        const auto index = get_index_of_first_overlapping_cursor(c);
        if(index)
        {
            // two selections overlap, merge and use the latest selection for order
            const auto lhs_cursor = new_cursors[*index].as_sorted();
            const auto rhs_cursor = c.as_sorted();
            const auto lower = std::min(lhs_cursor.a, rhs_cursor.a);
            const auto upper = std::max(lhs_cursor.b, rhs_cursor.b);
            const auto merged = c.a < c.b ? Selection{lower, upper} : Selection{upper, lower};
            new_cursors[*index] = merged;
        }
        else
        {
            new_cursors.emplace_back(c);
        }
    }

    cursors = new_cursors;
}


void Document::add_text(const std::string& text, const Position& pp)
{
    for(auto* view: views)
    {
        view->sanitize_cursors();
    }

    const auto p = sanitize_position(pp);

    auto more_lines = split_string_by_newline(text);
    const auto before = this->lines[Cs(p.line)].substr(0, Cs(p.offset));
    const auto after = this->lines[Cs(p.line)].substr(Cs(p.offset));
    more_lines[0] = before + more_lines[0];
    *more_lines.rbegin() += after;
    
    // splice lines into line array
    splice(&lines, p.line, 1, more_lines);

    for(auto* view: views)
    {
        view->advance_cursors(p, C(text.size()));
    }
}

void Document::load_lines(const std::vector<std::string>& new_lines)
{
    lines = new_lines;
}

void VirtualView::set_document(std::shared_ptr<Document> new_document)
{
    if(doc != nullptr)
    {
        erase_remove_if(&doc->views, [this](VirtualView* other) -> bool { return other == this; } );
    }
    doc = new_document;
    if(doc != nullptr)
    {
        doc->views.emplace_back(this);
    }
}

VirtualView::~VirtualView()
{
    set_document(nullptr);
}

void VirtualView::sanitize_cursors()
{
    for(auto& c: cursors)
    {
        c.a = doc->sanitize_position(c.a);
        c.b = doc->sanitize_position(c.b);
    }
}

void VirtualView::advance_cursors(const Position& after, int offset)
{
    auto advance_position = [&after, &offset, this](Position* p)
    {
        if(*p >= after && p->line == after.line)
        {
            if(*p != end_of_line(*p))
            {
                *p = doc->position_offset(*p, offset);
            }
        }
    };

    for(auto& c: cursors)
    {
        advance_position(&c.a);
        advance_position(&c.b);
    }
}

void VirtualView::insert_text_at_cursors(const std::string& text)
{
    if(doc == nullptr) { return; }
    if(cursors.empty()) { return; }
    for(auto& c: cursors)
    {
        doc->add_text(text, c.b);
    }
}


}

