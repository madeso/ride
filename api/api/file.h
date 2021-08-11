#pragma once

#include <cstdint>
#include <vector>
#include <string>

bool read_to_buffer(const std::string& filename, std::vector<std::uint8_t>* buffer);
