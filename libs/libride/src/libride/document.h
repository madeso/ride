#pragma once

#include <vector>
#include <string>
#include <optional>
#include <limits>

struct filesystem;

struct position
{
    int line;
    int offset;

    // static constexpr int max_line = std::numeric_limits<int>::max();
    static constexpr int max_offset = std::numeric_limits<int>::max();
};

bool operator==(const position& lhs, const position& rhs);
bool operator!=(const position& lhs, const position& rhs);
bool operator<(const position& lhs, const position& rhs);
bool operator>(const position& lhs, const position& rhs);

bool operator<=(const position& lhs, const position& rhs);
bool operator>=(const position& lhs, const position& rhs);

struct sorted_selection
{
    position a;
    position b;
};

struct selection
{
    position a;
    position b;

    sorted_selection sorted() const;
    bool is_selection() const;
};


bool are_overlapping(const sorted_selection& s, const sorted_selection& p);
bool are_overlapping(const selection& sel, const selection& p);

struct Document
{
    std::optional<std::string> path_or_not;
    std::vector<std::string> lines;
    std::vector<selection> cursors;
    
    bool LoadFile(filesystem* fs, const std::string& path);


    char get_char(const position& p) const;
    position sanitize_position(const position& pp) const;
    position position_offset(const position& pp, int offset) const;

    int GetNumberOfLines() const;
    std::string GetLineAt(int y) const;

    void merge_all_cursors();
};
