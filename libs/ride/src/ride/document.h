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

struct FileSystem;
struct VirtualView;

struct Position
{
    int line;
    int offset;

    // static constexpr int max_line = std::numeric_limits<int>::max();
    static constexpr int max_offset = std::numeric_limits<int>::max();
};

template<> struct fmt::formatter<Position>: formatter<string_view>
{
    template <typename FormatContext>
    auto format(Position p, FormatContext& ctx)
    {
        return format_to(ctx.out(), "{} ({})", p.line, p.offset);
    }
};


bool operator==(const Position& lhs, const Position& rhs);
bool operator!=(const Position& lhs, const Position& rhs);
bool operator<(const Position& lhs, const Position& rhs);
bool operator>(const Position& lhs, const Position& rhs);

bool operator<=(const Position& lhs, const Position& rhs);
bool operator>=(const Position& lhs, const Position& rhs);

std::ostream& operator<<(std::ostream& s, const Position& p);

struct SortedSelection
{
    // todo(Gustav): rename to min+max, not something like lower+upper as lower in the file is a higher line index
    Position a;
    Position b;
};

struct Selection
{
    // todo(Gustav): rename to from+to, start+end
    Position a;
    Position b;

    SortedSelection as_sorted() const;
    bool is_selection() const;
};

bool operator==(const Selection& lhs, const Selection& rhs);
bool operator!=(const Selection& lhs, const Selection& rhs);


template<> struct fmt::formatter<Selection>: formatter<string_view>
{
    template <typename FormatContext>
    auto format(Selection p, FormatContext& ctx)
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


bool are_overlapping(const SortedSelection& s, const SortedSelection& p);
bool are_overlapping(const Selection& sel, const Selection& p);

struct Document
{
    std::optional<std::string> path_or_not;
    private:
    std::vector<std::string> lines;
    public:
    std::vector<VirtualView*> views;
    
    bool load_file(FileSystem* fs, const std::string& path);
    void load_lines(const std::vector<std::string>& new_lines);

    char get_char(const Position& p) const;

    int sanitize_line(int new_line) const;
    Position sanitize_position(const Position& pp) const;
    Position position_offset(const Position& pp, int offset) const;

    int get_number_of_lines() const;
    std::string get_line_at(int y) const;

    void add_text(const std::string& t, const Position& p);
};

struct VirtualView
{
    std::shared_ptr<Document> doc;
    void set_document(std::shared_ptr<Document> new_document);

    virtual ~VirtualView();

    std::vector<Selection> cursors;
    void merge_all_cursors();
    void sanitize_cursors();

    void advance_cursors(const Position& after, int offset);
    void insert_text_at_cursors(const std::string& t);

    virtual void scroll_to_cursor(const Position& p) = 0;

    virtual Dp get_relative_pixel_offset(const Position& p) = 0;
    virtual int get_offset_from_relative_pixel_offset(int line, Dp offset) = 0;
};
