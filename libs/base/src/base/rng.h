#pragma once

#include <random>

struct Rng
{
    std::default_random_engine engine;

    static std::default_random_engine create();

    Rng();

    template <typename T>
    T generate(T max = std::numeric_limits<T>::max(), T min = std::numeric_limits<T>::min())
    {
        std::uniform_int_distribution<T> uniform_dist(min, max);
        return uniform_dist(engine);
    }
};
