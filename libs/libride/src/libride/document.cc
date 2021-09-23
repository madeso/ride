#include "libride/document.h"

#include "base/c.h"
#include "base/filesystem.h"

bool Document::LoadFile(filesystem* fs, const std::string& path)
{
    lines.clear();
    return fs->load_file
    (
        path,
        [this](const std::string& line)
        {
            lines.emplace_back(line);
        }
    );
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
