#pragma once

#include <random>

#include "core/ints.h"


namespace ride
{


struct Rng
{
    std::default_random_engine engine;

    Rng();

    u8 generate_u8();
};


}

