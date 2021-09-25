#include "base/rng.h"

std::default_random_engine Rng::create()
{
    std::random_device random_device;
    return std::default_random_engine(random_device());
}

Rng::Rng()
    : engine(create())
{
}
