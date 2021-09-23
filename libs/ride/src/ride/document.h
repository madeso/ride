#pragma once

#include <vector>
#include <string>

struct filesystem;

struct Document
{
    std::vector<std::string> lines;
    
    bool LoadFile(filesystem* fs, const std::string& path);

    int GetNumberOfLines() const;
    std::string GetLineAt(int y) const;
};
