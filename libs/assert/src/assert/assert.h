#pragma once

#include <cassert>

#define XASSERT(x, m) do { if((x) == false){ fmt::print(stderr, "assert failure({}): {}\n", #x, m); assert(false && "failure"); }} while(false)
#define ASSERT(x) do { if((x) == false){ fmt::print(stderr, "assert failure({})\n", #x); assert(false && "failure"); }} while(false)
#define DIE(m) do { fmt::print(stderr, "assert failure: {}\n", m); assert(false && "failure"); } while(false)
