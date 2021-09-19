#pragma once

#include <vector>
#include <string>

struct Document
{
    std::vector<std::string> lines;
    
    void LoadFile(const std::string& path);

    int GetNumberOfLines() const;
    std::string GetLineAt(int y) const;
};
