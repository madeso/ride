#pragma once

#include "base/ints.h"

std::vector<u32> utf8_to_codepoints(const std::string& text);

std::vector<std::string> utf8_chars(const std::string& text);

bool is_utf8_cont(u8 byte);
bool is_utf8_cont_char(char c);
