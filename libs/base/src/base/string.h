#pragma once

#include <vector>
#include <string>
#include <string_view>

// returns the number of replacements made
std::size_t replace_all_inplace(std::string& inout, std::string_view what, std::string_view with);

std::string replace_all_f(const std::string& inout, std::string_view what, std::string_view with);

std::string to_lower(const std::string& str);

std::string trim_right(const std::string& string_to_trim, const std::string& trim_characters);
std::string trim_left(const std::string& string_to_trim, const std::string& trim_characters);
std::string trim(const std::string& string_to_trim, const std::string& trim_characters = " \t\r\n");
std::vector<std::string> split_on_delim(const std::string& s, char delim);

bool is_only_whitepace(const std::string& str);

int index_of_non_whitespace(const std::string& str);

std::vector<std::string> supersplit(const std::string& str, char delim);
std::vector<std::string> split_string_by_newline(const std::string& str);
