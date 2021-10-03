#pragma once

#include <vector>
#include <string>
#include <optional>
#include <limits>
#include <functional>
#include <memory>

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
    std::vector<VirtualView*> views;
    
    bool LoadFile(filesystem* fs, const std::string& path);

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

    std::vector<selection> cursors;
    void merge_all_cursors();

    void advance_cursors(const position& after, int offset);
    void insert_text_at_cursors(const std::string& t);

    virtual ~VirtualView() = default;
    virtual void scroll_to_cursor(const position& p) = 0;

    virtual pix get_relative_pixel_offset(const position& p) = 0;
    virtual int get_offset_from_relative_pixel_offset(int line, pix offset) = 0;
};
