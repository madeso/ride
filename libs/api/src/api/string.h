#pragma once

#include <string>
#include <string_view>

std::size_t replace_all(std::string& inout, std::string_view what, std::string_view with);

std::string to_lower(const std::string& str);
