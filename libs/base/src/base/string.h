#pragma once

#include <vector>
#include <string>
#include <string_view>

std::size_t replace_all(std::string& inout, std::string_view what, std::string_view with);

std::string to_lower(const std::string& str);

std::string TrimRight(const std::string& string_to_trim, const std::string& trim_characters);
std::string TrimLeft(const std::string& string_to_trim, const std::string& trim_characters);
std::string Trim(const std::string& string_to_trim, const std::string& trim_characters = " \t\r\n");
std::vector<std::string> Split(const std::string& s, char delim);
