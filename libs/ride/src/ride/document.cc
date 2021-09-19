#include "ride/document.h"

#include <fstream>

#include "base/c.h"

void Document::LoadFile(const std::string& path)
{
    lines.clear();

    std::ifstream file;
    file.open(path.c_str(), std::ios::in | std::ios::binary);
    if(file.is_open() == false)
    {
        return;
    }

    std::string line;
    while(std::getline(file,line))
    {
        lines.emplace_back(line);
    }
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
