#include "apiact/uniform.h"

namespace ride::apigl
{

Uniform::Uniform()
    : name("<unknown>")
    , location(-1)
    , debug_shader_program(0)
{
}


Uniform::Uniform(const std::string& n, int l, unsigned int sp)
    : name(n)
    , location(l)
    , debug_shader_program(sp)
{
}


Uniform::operator bool() const
{
    return is_valid();
}


bool
Uniform::is_valid() const
{
    return location >= 0;
}

}

