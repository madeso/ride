#pragma once

#include <random>

#include "base/ints.h"

struct Rng
{
    std::default_random_engine engine;

    Rng();

    u8 generate_u8();
};
