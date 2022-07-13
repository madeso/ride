#pragma once

#include <cstddef>


constexpr int C(std::size_t i) { return static_cast<int>(i); }
constexpr int Csizet_to_int(std::size_t i) { return static_cast<int>(i); }

constexpr std::size_t Cs(int i)  { return static_cast<size_t>(i); }
constexpr std::size_t Cint_to_sizet(int i)  { return static_cast<size_t>(i); }

constexpr float Cint_to_float(int i)  { return static_cast<float>(i); }

constexpr int c_uint_to_int(unsigned int i) { return static_cast<int>(i); }
