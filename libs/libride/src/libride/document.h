#pragma once

#include <vector>
#include <string>
#include <optional>

struct filesystem;

struct position
{
    int line;
    int offset;
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

struct Document
{
    std::optional<std::string> path_or_not;
    std::vector<std::string> lines;
    std::vector<selection> cursors;
    
    bool LoadFile(filesystem* fs, const std::string& path);

    int GetNumberOfLines() const;
    std::string GetLineAt(int y) const;
};
