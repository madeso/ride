#include "base/rng.h"

namespace
{
    std::default_random_engine create()
    {
        std::random_device random_device;
        return std::default_random_engine(random_device());
    }
}

Rng::Rng()
    : engine(create())
{
}

#if 0
template <typename T>
T generate(T max = std::numeric_limits<T>::max(), T min = std::numeric_limits<T>::min())
{
    std::uniform_int_distribution<T> uniform_dist(min, max);
    return uniform_dist(engine);
}
#endif

u8 Rng::generate_u8()
{
    // use int here because msvc doesn't support u8 as the uniform_dist
    const auto max = std::numeric_limits<u8>::max();
    std::uniform_int_distribution<int> uniform_dist(0, max);
    const auto r = uniform_dist(engine);
    return static_cast<u8>(r);
}
