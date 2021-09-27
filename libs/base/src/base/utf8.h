#pragma once

#include <vector>
#include <string>
#include "base/ints.h"

std::vector<u32> utf8_to_codepoints(const std::string& text);

std::vector<std::string> utf8_chars(const std::string& text);
