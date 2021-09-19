#include "base/file.h"

#include <fstream>

bool read_to_buffer(const std::string& filename, std::vector<std::uint8_t>* buffer)
{
    std::ifstream infile(filename.c_str());

    if (infile.good() == false)
    {
        return false;
    }

    // get length of file
    infile.seekg(0, std::ios::end);
    size_t length = infile.tellg();
    infile.seekg(0, std::ios::beg);

    buffer->resize(length);
    infile.read(reinterpret_cast<char*>(buffer->data()), length);

    return true;
}
