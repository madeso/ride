#pragma once

#include <cassert>
#include <iostream>

#define xassert(x, m) do { if((x) == false){ std::cerr << "assert failure(" #x "): "<< m << "\n"; assert(false && "failure"); }} while(false)
