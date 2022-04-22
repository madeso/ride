#pragma once

#include <vector>
#include <string>
#include <optional>
#include <limits>
#include <functional>
#include <memory>
#include <ostream>
#include "fmt/format.h"

#include "base/units.h"

struct filesystem;
struct VirtualView;

struct position
{
    int line;
    int offset;

    // static constexpr int max_line = std::numeric_limits<int>::max();
    static constexpr int max_offset = std::numeric_limits<int>::max();
};

template<> struct fmt::formatter<position>: formatter<string_view>
{
    template <typename FormatContext>
    auto format(position p, FormatContext& ctx)
    {
        return format_to(ctx.out(), "{} ({})", p.line, p.offset);
    }
};


bool operator==(const position& lhs, const position& rhs);
bool operator!=(const position& lhs, const position& rhs);
bool operator<(const position& lhs, const position& rhs);
bool operator>(const position& lhs, const position& rhs);

bool operator<=(const position& lhs, const position& rhs);
bool operator>=(const position& lhs, const position& rhs);

std::ostream& operator<<(std::ostream& s, const position& p);

struct sorted_selection
{
    // todo(Gustav): rename to min+max, not something like lower+upper as lower in the file is a higher line index
    position a;
    position b;
};

struct selection
{
    // todo(Gustav): rename to from+to, start+end
    position a;
    position b;

    sorted_selection sorted() const;
    bool is_selection() const;
};

bool operator==(const selection& lhs, const selection& rhs);
bool operator!=(const selection& lhs, const selection& rhs);


template<> struct fmt::formatter<selection>: formatter<string_view>
{
    template <typename FormatContext>
    auto format(selection p, FormatContext& ctx)
    {
        if(p.a != p.b)
        {
            return format_to(ctx.out(), "({}, {})", p.a, p.b);
        }
        else
        {
            return format_to(ctx.out(), "({})", p.a);
        }
    }
};


bool are_overlapping(const sorted_selection& s, const sorted_selection& p);
bool are_overlapping(const selection& sel, const selection& p);

struct Document
{
    std::optional<std::string> path_or_not;
    private:
    std::vector<std::string> lines;
    public:
    std::vector<VirtualView*> views;
    
    bool LoadFile(filesystem* fs, const std::string& path);
    void load_lines(const std::vector<std::string>& new_lines);

    char get_char(const position& p) const;

    int sanitize_line(int new_line) const;
    position sanitize_position(const position& pp) const;
    position position_offset(const position& pp, int offset) const;

    int GetNumberOfLines() const;
    std::string GetLineAt(int y) const;

    void add_text(const std::string& t, const position& p);
};

struct VirtualView
{
    std::shared_ptr<Document> doc;
    void set_document(std::shared_ptr<Document> new_document);

    virtual ~VirtualView();

    std::vector<selection> cursors;
    void merge_all_cursors();
    void sanitize_cursors();

    void advance_cursors(const position& after, int offset);
    void insert_text_at_cursors(const std::string& t);

    virtual void scroll_to_cursor(const position& p) = 0;

    virtual pix get_relative_pixel_offset(const position& p) = 0;
    virtual int get_offset_from_relative_pixel_offset(int line, pix offset) = 0;
};
