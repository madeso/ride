#pragma once

#include <cstddef>

constexpr int C(std::size_t i)  { return static_cast<int>(i); }
constexpr std::size_t Cs(int i)  { return static_cast<size_t>(i); }
