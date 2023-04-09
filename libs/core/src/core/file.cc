#include "core/file.h"

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
    const auto length = infile.tellg();
    const std::size_t length_t = static_cast<std::size_t>(length);
    infile.seekg(0, std::ios::beg);

    buffer->resize(length_t);
    infile.read(reinterpret_cast<char*>(buffer->data()), length);

    return true;
}
